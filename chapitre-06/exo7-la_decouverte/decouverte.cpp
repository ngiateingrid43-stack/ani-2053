
 // Exercice 7 - La decouverte
 // Role serveur : diffuse sa description toutes les deux secondes.
 // Role client  : ecoute six secondes, affiche les serveurs trouves, plusieurs fois.
 //
 // Usage :
 //   decouverte serveur [nom] [secondes]
 //   decouverte client [nombre_d_ecoutes]
 //
 // Le client ecrit serveurs-liste.log avec la liste et le comptage
 // des apparitions.

#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKNetwork/NKNetwork.h"
#include "NKNetwork/Lobby/NkLobby.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Decouverte";
    d.appVersion = "1.0.0";
    return d;
})());

// ------------------------------------------------------------
// ROLE SERVEUR
// ------------------------------------------------------------
static int RoleServeur(const char *nom, int secondes) {
    net::NkDiscovery::ServerInfo info{};

    // ServerInfo::name est un tableau char[128].
    // On copie le nom dans ce tableau.
    std::strncpy(info.name, nom, sizeof(info.name) - 1);
    info.name[sizeof(info.name) - 1] = '\0';

    info.maxPlayers = 8;

    logger.Infof(
        "[serveur] diffusion de \"%s\" (maxPlayers=%d) "
        "toutes les 2 s pendant %d s",
        info.name, info.maxPlayers, secondes
    );

    NkChrono total;
    int envois = 0;

    while (total.Elapsed().milliseconds < secondes * 1000.0) {
        const auto resultat = net::NkDiscovery::Broadcast(info);

        ++envois;

        logger.Infof(
            "[serveur] diffusion %d : resultat=%d",
            envois,
            static_cast<int>(resultat)
        );

        NkChrono::SleepMilliseconds(static_cast<int64>(2000));
    }

    logger.Infof("[serveur] fin des diffusions : %d envoi(s)", envois);
    return 0;
}

// ------------------------------------------------------------
// ROLE CLIENT
// ------------------------------------------------------------
static int RoleClient(int ecoutes) {
    FILE *journal = std::fopen("serveurs-liste.log", "w");

    if (journal == nullptr) {
        logger.Infof(
            "[client] Impossible de creer serveurs-liste.log"
        );
    }

    std::map<std::string, int> apparitions;

    for (int e = 1; e <= ecoutes; ++e) {
        NkVector<net::NkDiscovery::ServerInfo> trouves;

        // Ecoute pendant 6 secondes.
        const auto resultat = net::NkDiscovery::Listen(6000, trouves);

        logger.Infof(
            "[client] ecoute %d : %d entree(s), resultat=%d",
            e,
            static_cast<int>(trouves.Size()),
            static_cast<int>(resultat)
        );

        if (journal != nullptr) {
            std::fprintf(
                journal,
                "Ecoute %d : %d entree(s)\n",
                e,
                static_cast<int>(trouves.Size())
            );
        }

        for (usize i = 0; i < trouves.Size(); ++i) {
            // name est un tableau char[128] : pas de .CStr().
            const char *nom = trouves[i].name;

            // addr.ToString() renvoie ici un NkString,
            // dont CStr() donne le texte.
            const std::string adresse =
                trouves[i].addr.ToString().CStr();

            logger.Infof(
                "[client] adresse=%s nom=%s",
                adresse.c_str(),
                nom
            );

            if (journal != nullptr) {
                std::fprintf(
                    journal,
                    "  adresse=%s nom=%s\n",
                    adresse.c_str(),
                    nom
                );
            }

            ++apparitions[adresse + " (" + nom + ")"];
        }
    }

    // Ecriture du bilan dans le fichier.
    if (journal != nullptr) {
        std::fprintf(
            journal,
            "\nApparitions cumulees sur %d ecoute(s) :\n",
            ecoutes
        );

        for (const auto &kv : apparitions) {
            std::fprintf(
                journal,
                "  %s : %d fois\n",
                kv.first.c_str(),
                kv.second
            );
        }

        std::fclose(journal);
        logger.Infof("[client] bilan enregistre dans serveurs-liste.log");
    }

    // Affichage du bilan dans les logs.
    for (const auto &kv : apparitions) {
        logger.Infof(
            "[client] %s : %d fois",
            kv.first.c_str(),
            kv.second
        );
    }

    if (apparitions.empty()) {
        logger.Infof(
            "[client] Aucun serveur detecte pendant les ecoutes."
        );
    }

    return 0;
}

// ------------------------------------------------------------
// POINT D'ENTREE NKENTSEU
// ------------------------------------------------------------
int nkmain(const NkEntryState &state) {

    if (state.args.Size() < 2) {
        logger.Error(
            "[decouverte] usage : "
            "decouverte serveur [nom] [secondes] | "
            "decouverte client [ecoutes]"
        );
        return -1;
    }

    if (net::NkSocket::PlatformInit() !=
        net::NkNetResult::NK_NET_OK) {
        logger.Error("[decouverte] PlatformInit a echoue");
        return -2;
    }

    int code = 0;

    if (std::strcmp(state.args[1].CStr(), "serveur") == 0) {
        const char *nom =
            (state.args.Size() > 2)
                ? state.args[2].CStr()
                : "Salle-Test";

        const int secondes =
            (state.args.Size() > 3)
                ? std::atoi(state.args[3].CStr())
                : 40;

        if (secondes <= 0) {
            logger.Error(
                "[serveur] La duree doit etre superieure a zero."
            );
            code = -3;
        } else {
            code = RoleServeur(nom, secondes);
        }
    } else if (std::strcmp(state.args[1].CStr(), "client") == 0) {
        const int ecoutes =
            (state.args.Size() > 2)
                ? std::atoi(state.args[2].CStr())
                : 3;

        if (ecoutes <= 0) {
            logger.Error(
                "[client] Le nombre d'ecoutes doit etre superieur a zero."
            );
            code = -4;
        } else {
            code = RoleClient(ecoutes);
        }
    } else {
        logger.Error(
            "[decouverte] Role inconnu. Utilise serveur ou client."
        );
        code = -5;
    }

    net::NkSocket::PlatformShutdown();
    return code;
}
