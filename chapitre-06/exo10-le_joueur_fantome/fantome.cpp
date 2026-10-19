// Exercice 10 - Le joueur fantome
// Le serveur tient une table des joueurs connectes, note l'instant du dernier message de
// chacun, et dans onPeerDisconnected retire le joueur en affichant la raison et le delai
// depuis son dernier message. Le client envoie un message toutes les 100 ms.
//
// Usage :
//   fantome serveur
//   fantome client [adresse]           -> envoie en boucle : a tuer depuis le gestionnaire des taches
//   fantome client propre [adresse]    -> envoie 2 s puis appelle Shutdown() avant de quitter
// Le serveur ecrit fantome-serveur.log (a recopier dans fantome.txt).

#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKNetwork/NKNetwork.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <functional>
#include <thread>

using namespace nkentseu;
using namespace nkentseu::threading; // NkThread, NkThreadPool, NkMutex... vivent dans nkentseu::threading

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Fantome";
    d.appVersion = "1.0.0";
    return d;
})());

static const uint16 kPort = 7780;

// Identifiant du fil courant, pour savoir QUEL fil appelle chaque rappel.
static unsigned long long IdFil() {
    return static_cast<unsigned long long>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

static NkChrono g_horloge; // demarre avec le programme

static FILE *g_journal = nullptr;
static NkMutex g_mutexJournal;
static void Trace(const char *fmt, ...) {
    char ligne[512];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(ligne, sizeof(ligne), fmt, ap);
    va_end(ap);
    threading::NkScopedLock<NkMutex> verrou(g_mutexJournal);
    logger.Infof("%s", ligne);
    if (g_journal) {
        std::fprintf(g_journal, "%s\n", ligne);
        std::fflush(g_journal);
    }
}

// ---- Table des joueurs ----------------------------------------------------------------
// Touchee par DEUX fils : la boucle principale (messages recus) et le fil du moteur
// (rappels de connexion / deconnexion). Chaque acces prend donc g_mutexTable.
struct Joueur {
    unsigned long long id;
    float64 dernierMessageMs;
    bool present;
};
static const int32 kMaxJoueurs = 16;
static Joueur g_table[kMaxJoueurs];
static NkMutex g_mutexTable; // ordre des verrous : g_mutexTable puis g_mutexJournal (via Trace)

// A appeler verrou de la table DEJA pris.
static void AfficherTable(const char *titre) {
    char liste[256] = "";
    int32 n = 0;
    for (int32 i = 0; i < kMaxJoueurs; ++i) {
        if (!g_table[i].present) continue;
        char un[32];
        std::snprintf(un, sizeof(un), "%s%llu", n ? ", " : "", g_table[i].id);
        std::strncat(liste, un, sizeof(liste) - std::strlen(liste) - 1);
        ++n;
    }
    Trace("[serveur] table %s : %d joueur(s) [%s]", titre, static_cast<int>(n), liste);
}

static int RoleServeur() {
    net::NkConnectionManager serveur;
    serveur.onPeerConnected = [](net::NkPeerId p) {
        threading::NkScopedLock<NkMutex> verrou(g_mutexTable);
        for (int32 i = 0; i < kMaxJoueurs; ++i)
            if (!g_table[i].present) {
                g_table[i] = Joueur{p.value, g_horloge.Elapsed().milliseconds, true};
                break;
            }
        Trace("[serveur] connexion du joueur %llu (rappel appele par le fil %llu)", static_cast<unsigned long long>(p.value),
              IdFil());
        AfficherTable("apres connexion");
    };
    serveur.onPeerDisconnected = [](net::NkPeerId p, const char *raison) {
        threading::NkScopedLock<NkMutex> verrou(g_mutexTable);
        const float64 maintenant = g_horloge.Elapsed().milliseconds;
        float64 delai = -1.0;
        AfficherTable("avant depart");
        for (int32 i = 0; i < kMaxJoueurs; ++i)
            if (g_table[i].present && g_table[i].id == static_cast<unsigned long long>(p.value)) {
                delai = maintenant - g_table[i].dernierMessageMs;
                g_table[i].present = false; // le joueur est retire de la table
                break;
            }
        Trace("[serveur] DEPART du joueur %llu : raison = \"%s\", delai depuis son dernier message = %.1f ms, rappel appele par le fil %llu",
              static_cast<unsigned long long>(p.value), raison ? raison : "(aucune)", delai, IdFil());
        AfficherTable("apres depart");
    };

    const net::NkNetResult r = serveur.StartServer(kPort, 8);
    if (r != net::NkNetResult::NK_NET_OK) {
        Trace("[serveur] StartServer a echoue : %s", net::NkNetResultStr(r));
        return -2;
    }
    Trace("[serveur] a l'ecoute, boucle principale sur le fil %llu", IdFil());

    NkVector<net::NkReceiveMsg> msgs;
    NkChrono total;
    while (total.Elapsed().milliseconds < 600000.0) { // 10 minutes au plus
        msgs.Clear();
        serveur.DrainAll(msgs);
        if (msgs.Size() > 0) {
            threading::NkScopedLock<NkMutex> verrou(g_mutexTable);
            const float64 maintenant = g_horloge.Elapsed().milliseconds;
            for (usize i = 0; i < msgs.Size(); ++i)
                for (int32 j = 0; j < kMaxJoueurs; ++j)
                    if (g_table[j].present && g_table[j].id == static_cast<unsigned long long>(msgs[i].from.value))
                        g_table[j].dernierMessageMs = maintenant; // instant du dernier message recu
        }
        NkChrono::SleepMilliseconds(static_cast<int64>(5));
    }
    serveur.Shutdown();
    return 0;
}

static NkMutex g_mutexPair;
static net::NkPeerId g_pairServeur;

static int RoleClient(const char *adresse, bool propre) {
    net::NkConnectionManager client;
    client.onPeerConnected = [](net::NkPeerId p) {
        threading::NkScopedLock<NkMutex> verrou(g_mutexPair);
        g_pairServeur = p;
    };
    const net::NkAddress addr(adresse, kPort);
    if (!addr.IsValid() || client.Connect(addr) != net::NkNetResult::NK_NET_OK) {
        logger.Errorf("[client] connexion impossible a %s", adresse);
        return -2;
    }
    bool connecte = false;
    for (int32 i = 0; i < 500 && !connecte; ++i) {
        connecte = (client.ConnectedPeerCount() >= 1);
        if (!connecte) NkChrono::SleepMilliseconds(static_cast<int64>(10));
    }
    if (!connecte) {
        logger.Error("[client] pas de connexion apres 5 secondes");
        client.Shutdown();
        return -3;
    }
    NkChrono::SleepMilliseconds(static_cast<int64>(200));
    net::NkPeerId pair;
    {
        threading::NkScopedLock<NkMutex> verrou(g_mutexPair);
        pair = g_pairServeur;
    }
    logger.Infof("[client] connecte, mode %s (processus a tuer depuis le gestionnaire des taches si 'coupure')",
                 propre ? "propre" : "coupure");

    const char *texte = "ping";
    NkChrono duree;
    while (!propre || duree.Elapsed().milliseconds < 2000.0) {
        client.SendTo(pair, reinterpret_cast<const uint8 *>(texte), static_cast<uint32>(std::strlen(texte) + 1),
                      net::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED);
        NkChrono::SleepMilliseconds(static_cast<int64>(100));
    }
    client.Shutdown(); // fermeture propre : le serveur est prevenu
    logger.Info("[client] Shutdown() appele, fermeture propre");
    return 0;
}

int nkmain(const NkEntryState &state) {
    if (state.args.Size() < 2) {
        logger.Error("[fantome] usage : fantome serveur | fantome client [propre] [adresse]");
        return -1;
    }
    const bool serveur = (std::strcmp(state.args[1].CStr(), "serveur") == 0);
    bool propre = false;
    const char *adresse = "127.0.0.1";
    for (usize i = 2; i < state.args.Size(); ++i) {
        if (std::strcmp(state.args[i].CStr(), "propre") == 0) propre = true;
        else adresse = state.args[i].CStr();
    }
    if (serveur) g_journal = std::fopen("fantome-serveur.log", "w");
    if (net::NkSocket::PlatformInit() != net::NkNetResult::NK_NET_OK) return -2;
    const int code = serveur ? RoleServeur() : RoleClient(adresse, propre);
    net::NkSocket::PlatformShutdown();
    if (g_journal) std::fclose(g_journal);
    return code;
}
