#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKNetwork/NKNetwork.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Parle";
    d.appVersion = "1.0.0";
    return d;
})());

static const uint16 kPort = 7777;

static FILE *g_journal = nullptr;
static NkMutex g_mutexJournal;

// Affiche a la console ET ecrit dans le fichier journal (appelable depuis le fil reseau).
static void Trace(const char *fmt, ...) {
    char ligne[512];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(ligne, sizeof(ligne), fmt, ap);
    va_end(ap);
    NkScopedLock<NkMutex> verrou(g_mutexJournal);
    logger.Infof("%s", ligne);
    if (g_journal) {
        std::fprintf(g_journal, "%s\n", ligne);
        std::fflush(g_journal);
    }
}

// Identifiant du serveur vu par le client : ecrit par le fil reseau, lu par la boucle principale.
static NkMutex g_mutexPair;
static net::NkPeerId g_pairServeur;
static bool g_pairConnu = false;

static int RoleServeur() {
    net::NkConnectionManager serveur;
    serveur.onPeerConnected = [](net::NkPeerId p) {
        Trace("[serveur] pair connecte, identifiant (value) = %llu", static_cast<unsigned long long>(p.value));
    };
    serveur.onPeerDisconnected = [](net::NkPeerId p, const char *raison) {
        Trace("[serveur] pair %llu deconnecte (%s)", static_cast<unsigned long long>(p.value), raison ? raison : "sans raison");
    };
    const net::NkNetResult r = serveur.StartServer(kPort, 8);
    if (r != net::NkNetResult::NK_NET_OK) {
        Trace("[serveur] StartServer a echoue : %s", net::NkNetResultStr(r));
        return -2;
    }
    Trace("[serveur] a l'ecoute sur le port %d", static_cast<int>(kPort));

    NkVector<net::NkReceiveMsg> msgs;
    bool recu = false;
    NkChrono attente;
    while (!recu && attente.Elapsed().milliseconds < 120000.0) { // borne : 2 minutes
        msgs.Clear(); // DrainAll AJOUTE : on vide avant chaque tour
        serveur.DrainAll(msgs);
        for (usize i = 0; i < msgs.Size(); ++i) {
            const net::NkReceiveMsg &m = msgs[i];
            if (m.size == 0) continue;
            char texte[net::kNkMaxPayloadSize + 1];
            const uint32 n = (m.size < net::kNkMaxPayloadSize) ? m.size : net::kNkMaxPayloadSize;
            std::memcpy(texte, m.data, n);
            texte[n] = '\0';
            Trace("[serveur] message recu de l'expediteur (from.value) = %llu : \"%s\"",
                  static_cast<unsigned long long>(m.from.value), texte);
            recu = true;
        }
        NkChrono::SleepMilliseconds(static_cast<int64>(10));
    }
    NkChrono::SleepMilliseconds(static_cast<int64>(500));
    serveur.Shutdown();
    return recu ? 0 : -3;
}

static int RoleClient(const char *adresse) {
    net::NkConnectionManager client;
    client.onPeerConnected = [](net::NkPeerId p) {
        {
            NkScopedLock<NkMutex> verrou(g_mutexPair);
            g_pairServeur = p;
            g_pairConnu = true;
        }
        Trace("[client] connecte, identifiant du serveur (value) = %llu", static_cast<unsigned long long>(p.value));
    };

    const net::NkAddress addr(adresse, kPort);
    if (!addr.IsValid()) {
        Trace("[client] adresse invalide : %s", adresse);
        return -2;
    }
    const net::NkNetResult r = client.Connect(addr);
    Trace("[client] Connect(%s) -> %s (la demande est partie, pas encore connecte)", addr.ToString().CStr(),
          net::NkNetResultStr(r));
    if (r != net::NkNetResult::NK_NET_OK) return -3;

    // La connexion se fait dans un fil du moteur : on attend, boucle bornee (5 s).
    bool connecte = false;
    for (int32 i = 0; i < 500; ++i) {
        if (client.ConnectedPeerCount() >= 1) {
            connecte = true;
            break;
        }
        NkChrono::SleepMilliseconds(static_cast<int64>(10));
    }
    if (!connecte) {
        Trace("[client] pas de connexion apres 5 secondes");
        client.Shutdown();
        return -4;
    }
    NkChrono::SleepMilliseconds(static_cast<int64>(200)); // laisser le rappel s'executer

    net::NkPeerId pair;
    {
        NkScopedLock<NkMutex> verrou(g_mutexPair);
        pair = g_pairServeur;
    }
    const char *texte = "bonjour serveur";
    const net::NkNetResult e = client.SendTo(pair, reinterpret_cast<const uint8 *>(texte),
                                             static_cast<uint32>(std::strlen(texte) + 1),
                                             net::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED);
    Trace("[client] message \"%s\" envoye -> %s", texte, net::NkNetResultStr(e));
    NkChrono::SleepMilliseconds(static_cast<int64>(500));
    client.Shutdown();
    return (e == net::NkNetResult::NK_NET_OK) ? 0 : -5;
}

int nkmain(const NkEntryState &state) {
    if (state.args.Size() < 2) {
        logger.Error("[parle] usage : parle serveur | parle client [adresse]");
        return -1;
    }
    const bool serveur = (std::strcmp(state.args[1].CStr(), "serveur") == 0);
    const char *adresse = (state.args.Size() > 2) ? state.args[2].CStr() : "127.0.0.1";

    g_journal = std::fopen(serveur ? "echange-serveur.log" : "echange-client.log", "w");
    if (net::NkSocket::PlatformInit() != net::NkNetResult::NK_NET_OK) {
        logger.Error("[parle] PlatformInit a echoue");
        return -2;
    }
    const int code = serveur ? RoleServeur() : RoleClient(adresse);
    net::NkSocket::PlatformShutdown();
    if (g_journal) std::fclose(g_journal);
    return code;
}
