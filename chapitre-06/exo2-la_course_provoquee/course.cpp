#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKThreading/NkThread.h"

#include <cstdio>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::threading; // NkThread, NkThreadPool, NkMutex... vivent dans nkentseu::threading

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Course";
    d.appVersion = "1.0.0";
    return d;
})());

static const int32 kIncrements = 1000000;
static int32 g_compteur = 0; // partage, sans AUCUNE protection

static int64 LancerCourse(int32 nbFils) {
    g_compteur = 0;
    NkThread *fils[16];
    for (int32 i = 0; i < nbFils; ++i) {
        fils[i] = new NkThread([](void *) {
            for (int32 k = 0; k < kIncrements; ++k) g_compteur += 1;
        });
        fils[i]->SetName("compteur");
    }
    for (int32 i = 0; i < nbFils; ++i) fils[i]->Join(); // le total se lit APRES les Join
    for (int32 i = 0; i < nbFils; ++i) delete fils[i];
    return static_cast<int64>(g_compteur);
}

int nkmain(const NkEntryState &state) {
    const bool release = (state.args.Size() > 1 && std::strcmp(state.args[1].CStr(), "release") == 0);
    const int64 attendu = 8LL * kIncrements;

    if (release) {
        int64 t[3];
        for (int32 i = 0; i < 3; ++i) {
            t[i] = LancerCourse(8);
            logger.Infof("[course] release %d : total=%lld (attendu %lld)", static_cast<int>(i + 1),
                         static_cast<long long>(t[i]), static_cast<long long>(attendu));
        }
        FILE *out = std::fopen("totaux-release.txt", "w");
        if (!out) return -1;
        std::fprintf(out, "Release : %lld, %lld, %lld (attendu %lld à chaque lancement)\n", static_cast<long long>(t[0]),
                     static_cast<long long>(t[1]), static_cast<long long>(t[2]), static_cast<long long>(attendu));
        std::fclose(out);
        logger.Info("[course] totaux-release.txt ecrit : a coller dans totaux.txt, sous Release :");
        return 0;
    }

    int64 t[10];
    int64 mini = 0, maxi = 0;
    for (int32 i = 0; i < 10; ++i) {
        t[i] = LancerCourse(8);
        if (i == 0 || t[i] < mini) mini = t[i];
        if (i == 0 || t[i] > maxi) maxi = t[i];
        logger.Infof("[course] execution %d : total=%lld (attendu %lld)", static_cast<int>(i + 1),
                     static_cast<long long>(t[i]), static_cast<long long>(attendu));
    }
    const int64 seul = LancerCourse(1);
    logger.Infof("[course] un seul fil : total=%lld (attendu %d)", static_cast<long long>(seul), kIncrements);

    FILE *out = std::fopen("totaux.txt", "w");
    if (!out) return -2;
    std::fprintf(out, "Dix totaux : (total attendu : %lld = 8 fils x %d)\n", static_cast<long long>(attendu), kIncrements);
    for (int32 i = 0; i < 10; ++i) std::fprintf(out, "%lld\n", static_cast<long long>(t[i]));
    std::fprintf(out, "\nUn seul fil : %lld (attendu %d)\n\n", static_cast<long long>(seul), kIncrements);
    std::fprintf(out, "Release : \n\n");
    std::fprintf(out,
                 "Pourquoi : compteur += 1 n'est pas une seule opération : le processeur lit la valeur en mémoire, lui ajoute 1, puis réécrit le résultat. "
                 "Si deux fils lisent la même valeur avant que l'un ait réécrit, ils réécrivent tous les deux valeur + 1 : deux incréments, un seul effet. "
                 "Le nombre d'incréments perdus dépend de l'instant exact où chaque fil est interrompu, d'où des totaux différents à chaque exécution "
                 "(de %lld à %lld sur mes dix, au lieu de %lld). "
                 "Avec un seul fil, personne ne peut s'intercaler entre la lecture et l'écriture : le total est exact (%lld). "
                 "En Release, <<A_COMPLETER: décrire ce que vous avez observé et pourquoi. Deux explications à vérifier : le compilateur garde le compteur dans un registre pendant toute la boucle et n'écrit qu'à la fin, ou chaque fil termine avant que le suivant démarre>>.\n",
                 static_cast<long long>(mini), static_cast<long long>(maxi), static_cast<long long>(attendu),
                 static_cast<long long>(seul));
    std::fclose(out);
    return 0;
}
