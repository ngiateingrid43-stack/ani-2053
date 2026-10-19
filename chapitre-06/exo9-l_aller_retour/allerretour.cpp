// Exercice 9 - L'aller-retour
// Le serveur renvoie tel quel chaque message. Le client envoie 100 messages numerotes,
// UN A LA FOIS, chronometre chaque aller-retour, calcule moyenne / min / max, le
// RTT lisse (SRTT <- (1 - 0,125) x SRTT + 0,125 x R, depart 100 ms), et lit l'estimation
// du moteur (GetConnectionStats -> rttMs).
//
// Usage :
//   allerretour serveur
//   allerretour client [adresse]      (127.0.0.1 par defaut)
// Le client ecrit mesures-rtt.txt (100 lignes) et rtt.txt.

#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKNetwork/NKNetwork.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"

#include <cstdio>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::threading; // NkThread, NkThreadPool, NkMutex... vivent dans nkentseu::threading

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "AllerRetour";
    d.appVersion = "1.0.0";
    return d;
})());

static const uint16 kPort = 7779;
static const int32 kNbMesures = 100;

static NkMutex g_mutexPair;
static net::NkPeerId g_pairServeur;

static int RoleServeur() {
    net::NkConnectionManager serveur;
    if (serveur.StartServer(kPort, 4) != net::NkNetResult::NK_NET_OK) {
        logger.Error("[serveur] StartServer a echoue");
        return -2;
    }
    NkVector<net::NkReceiveMsg> msgs;
    NkChrono total;
    float64 dernierMessage = 0.0; // arret apres 10 s sans message
    while (total.Elapsed().milliseconds - dernierMessage < 10000.0 && total.Elapsed().milliseconds < 600000.0) {
        msgs.Clear();
        serveur.DrainAll(msgs);
        for (usize i = 0; i < msgs.Size(); ++i) {
            const net::NkReceiveMsg &m = msgs[i];
            // Renvoi au bon expediteur (le champ from), tel quel, meme canal.
            serveur.SendTo(m.from, m.data, m.size, m.channel);
            dernierMessage = total.Elapsed().milliseconds;
        }
        NkChrono::SleepMilliseconds(static_cast<int64>(0)); // cede la main sans retarder la reponse
    }
    serveur.Shutdown();
    return 0;
}

static int RoleClient(const char *adresse) {
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
    NkChrono::SleepMilliseconds(static_cast<int64>(300));
    net::NkPeerId pair;
    {
        threading::NkScopedLock<NkMutex> verrou(g_mutexPair);
        pair = g_pairServeur;
    }

    float64 rtt[kNbMesures];
    NkVector<net::NkReceiveMsg> msgs;
    for (int32 n = 0; n < kNbMesures; ++n) {
        bool recu = false;
        for (int32 essai = 0; essai < 3 && !recu; ++essai) {
            uint32 numero = static_cast<uint32>(n);
            NkChrono chrono; // demarre juste avant l'envoi
            client.SendTo(pair, reinterpret_cast<const uint8 *>(&numero), sizeof(numero),
                          net::NkNetChannel::NK_NET_CHANNEL_UNRELIABLE);
            // On attend le RETOUR du meme numero avant d'envoyer le suivant.
            while (!recu && chrono.Elapsed().milliseconds < 1000.0) {
                msgs.Clear();
                client.DrainAll(msgs);
                for (usize i = 0; i < msgs.Size(); ++i) {
                    uint32 recuNum = 0;
                    if (msgs[i].size >= sizeof(recuNum)) {
                        std::memcpy(&recuNum, msgs[i].data, sizeof(recuNum));
                        if (recuNum == numero) {
                            rtt[n] = chrono.Elapsed().milliseconds;
                            recu = true;
                        }
                    }
                }
            }
        }
        if (!recu) {
            logger.Errorf("[client] message %d perdu apres 3 essais", static_cast<int>(n));
            client.Shutdown();
            return -4;
        }
    }

    // Statistiques et lissage.
    float64 somme = 0.0, mini = rtt[0], maxi = rtt[0], srtt = 100.0;
    for (int32 n = 0; n < kNbMesures; ++n) {
        somme += rtt[n];
        if (rtt[n] < mini) mini = rtt[n];
        if (rtt[n] > maxi) maxi = rtt[n];
        srtt = (1.0 - 0.125) * srtt + 0.125 * rtt[n];
    }
    const float64 moyenne = somme / kNbMesures;

    net::NkConnectionStats stats;
    const bool ok = client.GetConnectionStats(pair, stats);
    const float64 moteur = ok ? static_cast<float64>(stats.rttMs) : -1.0;
    logger.Infof("[client] moyenne=%.3f min=%.3f max=%.3f lissee=%.3f moteur=%.3f (ms)", moyenne, mini, maxi, srtt, moteur);

    FILE *m = std::fopen("mesures-rtt.txt", "w");
    if (m) {
        for (int32 n = 0; n < kNbMesures; ++n) std::fprintf(m, "%.3f\n", rtt[n]);
        std::fclose(m);
    }
    FILE *out = std::fopen("rtt.txt", "w");
    if (out) {
        std::fprintf(out, "Moyenne : %.3f ms\n\n", moyenne);
        std::fprintf(out, "Minimum : %.3f ms\n\n", mini);
        std::fprintf(out, "Maximum : %.3f ms\n\n", maxi);
        std::fprintf(out, "Lissée : %.3f ms\n\n", srtt);
        std::fprintf(out, "Moteur : %.3f ms\n\n", moteur);
        std::fprintf(out,
                     "Écart : Mes mesures sont faites à chaque message, depuis la boucle du client, avec un chronomètre : la moyenne (%.3f ms) suit toutes les variations, le maximum (%.3f ms) montre un message plus lent que les autres. "
                     "Ma valeur lissée (%.3f ms) pondère chaque nouvelle mesure par 0,125 ; comme elle part de 100 ms, le départ pèse (1 - 0,125) puissance 100, soit environ 1,6 millionième de milliseconde au bout de cent mesures : il est complètement oublié, et la valeur lissée reflète surtout les dernières mesures. "
                     "L'estimation du moteur (%.3f ms) est faite par le fil réseau du moteur, pas par mon chronomètre : <<A_COMPLETER: lire NkConnectionManager::NetThreadLoop dans NkConnection.cpp et dire comment ce fil mesure et met à jour le RTT (à quels moments, avec quelle formule, si c'est une valeur entière) et pourquoi cela explique la différence avec mes mesures>>.\n",
                     moyenne, maxi, srtt, moteur);
        std::fclose(out);
    }
    client.Shutdown();
    return 0;
}

int nkmain(const NkEntryState &state) {
    if (state.args.Size() < 2) {
        logger.Error("[allerretour] usage : allerretour serveur | allerretour client [adresse]");
        return -1;
    }
    if (net::NkSocket::PlatformInit() != net::NkNetResult::NK_NET_OK) return -2;
    const bool serveur = (std::strcmp(state.args[1].CStr(), "serveur") == 0);
    const int code = serveur ? RoleServeur() : RoleClient((state.args.Size() > 2) ? state.args[2].CStr() : "127.0.0.1");
    net::NkSocket::PlatformShutdown();
    return code;
}
