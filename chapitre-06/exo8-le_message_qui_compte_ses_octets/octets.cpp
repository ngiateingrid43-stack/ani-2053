#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKNetwork/NKNetwork.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"

#include <cmath>
#include <cstdio>
#include <cstring>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Octets";
    d.appVersion = "1.0.0";
    return d;
})());

static const uint16 kPort = 7778;
static const float32 kPosMin = -500.f, kPosMax = 500.f, kPosPrec = 0.01f; // au centimetre
static const float32 kAngMin = 0.f, kAngMax = 360.f, kAngPrec = 0.5f;     // au demi-degre
static const int32 kSanteMin = 0, kSanteMax = 100;

struct Etat {
    bool vivant;
    uint8 numero;
    float32 x, y, angle;
    int32 sante;
};

static const Etat kEtat = {true, 200, 123.456f, -321.987f, 271.3f, 87};

// ---- Paquet complet : chaque champ dans son type entier -------------------------------
static uint32 EcrireComplet(const Etat &e, uint8 *buf, uint32 taille, uint32 *bits) {
    net::NkBitWriter w(buf, taille);
    w.WriteU8(e.vivant ? 1 : 0);
    w.WriteU8(e.numero);
    w.WriteF32(e.x);
    w.WriteF32(e.y);
    w.WriteF32(e.angle);
    w.WriteI32(e.sante);
    *bits = static_cast<uint32>(w.BitsWritten());
    return static_cast<uint32>(w.BytesWritten());
}

static Etat LireComplet(const uint8 *data, uint32 taille) {
    net::NkBitReader r(data, taille);
    Etat e;
    e.vivant = (r.ReadU8() != 0);
    e.numero = r.ReadU8();
    e.x = r.ReadF32();
    e.y = r.ReadF32();
    e.angle = r.ReadF32();
    e.sante = r.ReadI32();
    return e;
}

// ---- Paquet quantifie : memes champs, memes bornes a l'ecriture et a la lecture --------
static uint32 EcrireQuantifie(const Etat &e, uint8 *buf, uint32 taille, uint32 *bits) {
    net::NkBitWriter w(buf, taille);
    w.WriteBool(e.vivant);
    w.WriteU8(e.numero);
    w.WriteF32Q(e.x, kPosMin, kPosMax, kPosPrec);
    w.WriteF32Q(e.y, kPosMin, kPosMax, kPosPrec);
    w.WriteF32Q(e.angle, kAngMin, kAngMax, kAngPrec);
    w.WriteInt(e.sante, kSanteMin, kSanteMax);
    *bits = static_cast<uint32>(w.BitsWritten());
    w.AlignToByte();
    return static_cast<uint32>(w.BytesWritten());
}

static Etat LireQuantifie(const uint8 *data, uint32 taille) {
    net::NkBitReader r(data, taille);
    Etat e;
    e.vivant = r.ReadBool();
    e.numero = r.ReadU8();
    e.x = r.ReadF32Q(kPosMin, kPosMax, kPosPrec);
    e.y = r.ReadF32Q(kPosMin, kPosMax, kPosPrec);
    e.angle = r.ReadF32Q(kAngMin, kAngMax, kAngPrec);
    e.sante = r.ReadInt(kSanteMin, kSanteMax);
    return e;
}

static void AfficherEtat(const char *titre, const Etat &e) {
    logger.Infof("[octets] %s : vivant=%d numero=%d x=%.4f y=%.4f angle=%.4f sante=%d", titre, e.vivant ? 1 : 0,
                 static_cast<int>(e.numero), e.x, e.y, e.angle, static_cast<int>(e.sante));
}

// ---- Mode local : tailles, relecture, balayage, tailles.txt -----------------------------
static int ModeLocal() {
    uint8 b1[64], b2[64];
    uint32 bits1 = 0, bits2 = 0;
    const uint32 o1 = EcrireComplet(kEtat, b1, sizeof(b1), &bits1);
    const uint32 o2 = EcrireQuantifie(kEtat, b2, sizeof(b2), &bits2);
    logger.Infof("[octets] complet : %u octets (%u bits) ; quantifie : %u octets (%u bits)", o1, bits1, o2, bits2);

    const Etat r1 = LireComplet(b1, o1);
    const Etat r2 = LireQuantifie(b2, o2);
    AfficherEtat("envoye", kEtat);
    AfficherEtat("relu complet", r1);
    AfficherEtat("relu quantifie", r2);

    // Ecart maximal : balayage de 100 000 etats pseudo-aleatoires (generateur congruentiel).
    uint32 graine = 12345u;
    auto alea = [&graine]() {
        graine = graine * 1664525u + 1013904223u;
        return static_cast<float32>((graine >> 8) & 0xFFFFFFu) / 16777215.f; // 0..1
    };
    double maxX = 0.0, maxY = 0.0, maxAng = 0.0, maxSante = 0.0;
    for (int32 i = 0; i < 100000; ++i) {
        Etat e;
        e.vivant = (i % 2) == 0;
        e.numero = static_cast<uint8>(i & 255);
        e.x = kPosMin + alea() * (kPosMax - kPosMin);
        e.y = kPosMin + alea() * (kPosMax - kPosMin);
        e.angle = kAngMin + alea() * (kAngMax - kAngMin);
        e.sante = kSanteMin + static_cast<int32>(alea() * static_cast<float32>(kSanteMax - kSanteMin) + 0.5f);
        uint8 buf[64];
        uint32 bits = 0;
        const uint32 n = EcrireQuantifie(e, buf, sizeof(buf), &bits);
        const Etat r = LireQuantifie(buf, n);
        maxX = std::fmax(maxX, std::fabs(static_cast<double>(r.x) - e.x));
        maxY = std::fmax(maxY, std::fabs(static_cast<double>(r.y) - e.y));
        maxAng = std::fmax(maxAng, std::fabs(static_cast<double>(r.angle) - e.angle));
        maxSante = std::fmax(maxSante, std::fabs(static_cast<double>(r.sante) - e.sante));
    }
    const double ex = std::fabs(static_cast<double>(r2.x) - kEtat.x);
    const double ey = std::fabs(static_cast<double>(r2.y) - kEtat.y);
    const double ea = std::fabs(static_cast<double>(r2.angle) - kEtat.angle);
    logger.Infof("[octets] ecart sur l'etat envoye : x=%.5f y=%.5f angle=%.5f", ex, ey, ea);
    logger.Infof("[octets] ecart maximal (100000 etats) : x=%.5f y=%.5f angle=%.5f sante=%.1f", maxX, maxY, maxAng, maxSante);

    FILE *out = std::fopen("tailles.txt", "w");
    if (!out) return -1;
    std::fprintf(out, "Types complets : %u octets (%u bits)\n\n", o1, bits1);
    std::fprintf(out, "Quantifié : %u octets (%u bits)\n\n", o2, bits2);
    std::fprintf(out,
                 "Écart maximal : sur l'état envoyé : x %.5f m, y %.5f m, angle %.5f degré, santé %d, vivant et numéro exacts. "
                 "Plus grand écart sur un balayage de 100000 états aléatoires dans les bornes : x %.5f m, y %.5f m, angle %.5f degré, santé %.1f. "
                 "Les paquets en types complets se relisent sans aucun écart.\n\n",
                 ex, ey, ea, static_cast<int>(std::fabs(static_cast<double>(r2.sante) - kEtat.sante)), maxX, maxY, maxAng, maxSante);
    std::fprintf(out,
                 "Ce que le joueur verrait : si l'on prend l'ordre de grandeur d'un jeu où 1 mètre vaut environ 50 pixels, l'écart de position de %.3f cm au plus vaut moins d'un quart de pixel : on ne le voit pas, et le terrain de 1000 m par 1000 m tient sur 17 bits par axe au lieu de 32. "
                 "L'écart d'angle de %.3f degré au plus fait tourner un personnage d'un quart de degré : invisible à l'écran. "
                 "La santé est un entier exact et ne bouge pas. Le paquet passe de %u à %u octets, soit %.0f %% de moins, pour une erreur que le joueur ne peut pas voir ; la question qui guide n'est pas technique, c'est : quelle erreur le joueur peut-il voir ?\n",
                 maxX * 100.0, maxAng, o1, o2, 100.0 * (1.0 - static_cast<double>(o2) / static_cast<double>(o1)));
    std::fclose(out);
    logger.Info("[octets] tailles.txt ecrit");
    return 0;
}

// ---- Modes reseau ----------------------------------------------------------------------
static int ModeServeur() {
    net::NkConnectionManager serveur;
    const net::NkNetResult r = serveur.StartServer(kPort, 4);
    if (r != net::NkNetResult::NK_NET_OK) {
        logger.Errorf("[octets] StartServer : %s", net::NkNetResultStr(r));
        return -2;
    }
    uint8 b1[64], b2[64];
    uint32 bits = 0;
    const uint32 tailleComplet = EcrireComplet(kEtat, b1, sizeof(b1), &bits);
    const uint32 tailleQuant = EcrireQuantifie(kEtat, b2, sizeof(b2), &bits);

    NkVector<net::NkReceiveMsg> msgs;
    int recus = 0;
    NkChrono attente;
    while (recus < 2 && attente.Elapsed().milliseconds < 120000.0) {
        msgs.Clear();
        serveur.DrainAll(msgs);
        for (usize i = 0; i < msgs.Size(); ++i) {
            const net::NkReceiveMsg &m = msgs[i];
            if (m.size == tailleComplet) {
                logger.Infof("[octets] serveur : paquet COMPLET recu (%u octets)", m.size);
                AfficherEtat("relu complet", LireComplet(m.data, m.size));
                ++recus;
            } else if (m.size == tailleQuant) {
                logger.Infof("[octets] serveur : paquet QUANTIFIE recu (%u octets)", m.size);
                AfficherEtat("relu quantifie", LireQuantifie(m.data, m.size));
                ++recus;
            } else {
                logger.Infof("[octets] serveur : message de %u octets ignore", m.size);
            }
        }
        NkChrono::SleepMilliseconds(static_cast<int64>(10));
    }
    serveur.Shutdown();
    return recus == 2 ? 0 : -3;
}


static NkMutex g_mutexPair;
static net::NkPeerId g_pairServeur;

static int ModeClient(const char *adresse) {
    net::NkConnectionManager client;
    client.onPeerConnected = [](net::NkPeerId p) {
        NkScopedLock<NkMutex> verrou(g_mutexPair);
        g_pairServeur = p;
    };
    const net::NkAddress addr(adresse, kPort);
    if (!addr.IsValid() || client.Connect(addr) != net::NkNetResult::NK_NET_OK) {
        logger.Errorf("[octets] connexion impossible a %s", adresse);
        return -2;
    }
    bool connecte = false;
    for (int32 i = 0; i < 500 && !connecte; ++i) {
        connecte = (client.ConnectedPeerCount() >= 1);
        if (!connecte) NkChrono::SleepMilliseconds(static_cast<int64>(10));
    }
    if (!connecte) {
        logger.Error("[octets] pas de connexion apres 5 secondes");
        client.Shutdown();
        return -3;
    }
    NkChrono::SleepMilliseconds(static_cast<int64>(200));
    net::NkPeerId pair;
    {
        NkScopedLock<NkMutex> verrou(g_mutexPair);
        pair = g_pairServeur;
    }
    uint8 b1[64], b2[64];
    uint32 bits1 = 0, bits2 = 0;
    const uint32 o1 = EcrireComplet(kEtat, b1, sizeof(b1), &bits1);
    const uint32 o2 = EcrireQuantifie(kEtat, b2, sizeof(b2), &bits2);
    // Fiable et ordonne : le serveur recoit d'abord le paquet complet, puis le quantifie.
    const net::NkNetResult e1 = client.SendTo(pair, b1, o1, net::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED);
    const net::NkNetResult e2 = client.SendTo(pair, b2, o2, net::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED);
    logger.Infof("[octets] client : complet %u octets -> %s ; quantifie %u octets -> %s", o1, net::NkNetResultStr(e1), o2,
                 net::NkNetResultStr(e2));
    NkChrono::SleepMilliseconds(static_cast<int64>(1000));
    client.Shutdown();
    return 0;
}

int nkmain(const NkEntryState &state) {
    const char *mode = (state.args.Size() > 1) ? state.args[1].CStr() : "local";
    if (std::strcmp(mode, "local") == 0) return ModeLocal();
    if (net::NkSocket::PlatformInit() != net::NkNetResult::NK_NET_OK) return -1;
    int code;
    if (std::strcmp(mode, "serveur") == 0) {
        code = ModeServeur();
    } else {
        code = ModeClient((state.args.Size() > 2) ? state.args[2].CStr() : "127.0.0.1");
    }
    net::NkSocket::PlatformShutdown();
    return code;
}
