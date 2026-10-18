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

static int RoleServeur(const char *nom, int secondes) {
    net::NkDiscovery::ServerInfo info;
    info.name = NkString(nom);
    info.maxPlayers = 8; // 0 (valeur par defaut) serait ecarte sans message
    logger.Infof("[serveur] diffusion de \"%s\" (maxPlayers=%d) toutes les 2 s pendant %d s", nom, 8, secondes);
    NkChrono total;
    int envois = 0;
    while (total.Elapsed().milliseconds < secondes * 1000.0) {
        (void)net::NkDiscovery::Broadcast(info);
        ++envois;
        logger.Infof("[serveur] diffusion %d", envois);
        NkChrono::SleepMilliseconds(static_cast<int64>(2000));
    }
    return 0;
}

static int RoleClient(int ecoutes) {
    FILE *journal = std::fopen("serveurs-liste.log", "w");
    std::map<std::string, int> apparitions;
    for (int e = 1; e <= ecoutes; ++e) {
        NkVector<net::NkDiscovery::ServerInfo> trouves;
        net::NkDiscovery::Listen(6000, trouves); // bloque pendant six secondes : voulu ici
        logger.Infof("[client] ecoute %d : %d entree(s)", e, static_cast<int>(trouves.Size()));
        if (journal) std::fprintf(journal, "Ecoute %d : %d entree(s)\n", e, static_cast<int>(trouves.Size()));
        for (usize i = 0; i < trouves.Size(); ++i) {
            const std::string adresse = trouves[i].addr.ToString().CStr();
            const char *nom = trouves[i].name.CStr();
            logger.Infof("[client]   adresse=%s nom=%s", adresse.c_str(), nom);
            if (journal) std::fprintf(journal, "  adresse=%s nom=%s\n", adresse.c_str(), nom);
            ++apparitions[adresse + " (" + nom + ")"];
        }
    }
    if (journal) {
        std::fprintf(journal, "\nApparitions cumulees sur %d ecoute(s) :\n", ecoutes);
        for (const auto &kv : apparitions) std::fprintf(journal, "  %s : %d fois\n", kv.first.c_str(), kv.second);
        std::fclose(journal);
    }
    for (const auto &kv : apparitions) logger.Infof("[client] %s : %d fois", kv.first.c_str(), kv.second);
    return 0;
}

int nkmain(const NkEntryState &state) {
    if (state.args.Size() < 2) {
        logger.Error("[decouverte] usage : decouverte serveur [nom] [secondes] | decouverte client [ecoutes]");
        return -1;
    }
    if (net::NkSocket::PlatformInit() != net::NkNetResult::NK_NET_OK) {
        logger.Error("[decouverte] PlatformInit a echoue");
        return -2;
    }
    int code = 0;
    if (std::strcmp(state.args[1].CStr(), "serveur") == 0) {
        const char *nom = (state.args.Size() > 2) ? state.args[2].CStr() : "Salle-Test";
        const int s = (state.args.Size() > 3) ? std::atoi(state.args[3].CStr()) : 40;
        code = RoleServeur(nom, s);
    } else {
        const int n = (state.args.Size() > 2) ? std::atoi(state.args[2].CStr()) : 3;
        code = RoleClient(n);
    }
    net::NkSocket::PlatformShutdown();
    return code;
}
