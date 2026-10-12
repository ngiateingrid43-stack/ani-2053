// Exercice 1 - Le calcul, puis la mesure
// Charge cinq images avec NkImage::Load, calcule la memoire attendue
// (largeur x hauteur x octets par pixel), mesure la memoire reelle du processus
// avant/apres chaque chargement, et ecrit mesures.txt.
//
// Usage (depuis le dossier ou l'on veut obtenir mesures.txt) :
//   mesure image1 image2 image3 image4 image5

#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKImage/Core/NkImage.h"

#include <cstdio>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#elif defined(__linux__) || defined(__ANDROID__)
#include <unistd.h>
#endif

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Mesure";
    d.appVersion = "1.0.0";
    return d;
})());

static const int kNbImages = 5;

static const char *kChemins[kNbImages] = {
    "Ing.jpeg", "RIHEN Academy 09_10_2026 09_59_24.png", "icons8-ico-50.png", "WhatsApp Image 2026-10-06 at 00.45.44.jpeg","aplat.png" 
};

// Memoire privee du processus, en octets (-1 si la plateforme n'est pas geree).
static long long MemoireProcessus() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    pmc.cb = sizeof(pmc);
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&pmc),
                             sizeof(pmc)))
        return static_cast<long long>(pmc.PrivateUsage);
}

// Taille du fichier sur le disque, en octets (-1 si illisible).
static long long TailleFichier(const char *chemin) {
    FILE *f = std::fopen(chemin, "rb");
    if (!f) return -1;
    std::fseek(f, 0, SEEK_END);
    const long long t = static_cast<long long>(std::ftell(f));
    std::fclose(f);
    return t;
}

// Nom du fichier sans son dossier (on n'ecrit pas de chemin personnel dans le rendu).
static const char *NomSansDossier(const char *chemin) {
    const char *nom = chemin;
    for (const char *p = chemin; *p; ++p)
        if (*p == '/' || *p == '\\') nom = p + 1;
    return nom;
}

int nkmain(const NkEntryState &) {

    // Les cinq images restent vivantes jusqu'a la fin : on mesure pendant qu'elles vivent.
    NkImage images[kNbImages];
    long long calcul[kNbImages];
    long long mesure[kNbImages];
    long long fichier[kNbImages];

    for (int i = 0; i < kNbImages; ++i) {
        const char *chemin = kChemins[i];
        fichier[i] = TailleFichier(chemin);

        const long long avant = MemoireProcessus();
        if (!images[i].Load(chemin)) { // 0 canal demande : on garde le format du fichier
            logger.Errorf("[mesure] chargement impossible : %s", chemin);
            return -2;
        }
        const long long apres = MemoireProcessus();

        const long long l = static_cast<long long>(images[i].Width());
        const long long h = static_cast<long long>(images[i].Height());
        const long long bpp = static_cast<long long>(images[i].BytesPP());
        calcul[i] = l * h * bpp;
        mesure[i] = (avant >= 0 && apres >= 0) ? (apres - avant) : -1;

        logger.Infof("[mesure] %s : %lldx%lld, %lld octet(s)/pixel, calcul=%lld, mesure=%lld, fichier=%lld",
                     NomSansDossier(chemin), l, h, bpp, calcul[i], mesure[i], fichier[i]);
    }

    FILE *out = std::fopen("mesures.txt", "w");
    if (!out) {
        logger.Error("[mesure] impossible d'ecrire mesures.txt");
        return -3;
    }
    for (int i = 0; i < kNbImages; ++i) {
        const char *chemin = kChemins[i];
        std::fprintf(out, "image : %s, calcul : %lld, mesure : %lld, fichier : %lld\n", NomSansDossier(chemin),
                     calcul[i], mesure[i], fichier[i]);
    }
    std::fclose(out);
    logger.Info("[mesure] mesures.txt ecrit");
    return 0;
}
