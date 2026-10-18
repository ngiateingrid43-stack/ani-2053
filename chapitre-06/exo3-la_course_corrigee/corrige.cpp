#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKThreading/NkThread.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"
#include "NKCore/NkAtomic.h"

#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Corrige";
    d.appVersion = "1.0.0";
    return d;
})());

static const int32 kFils = 8;
static const int32 kIncrements = 1000000;

static int32 g_nu = 0;      // sans protection
static int32 g_mutexTotal = 0;
static NkMutex g_mutex;
static NkAtomicInt g_atomique;

struct alignas(64) Partiel { int64 valeur; }; // un par fil, sur sa propre ligne de cache
static Partiel g_partiels[kFils];

template <class F>
static float64 Mesurer(F corps) {
    NkThread *fils[kFils];
    NkChrono chrono;
    for (int32 i = 0; i < kFils; ++i) {
        fils[i] = new NkThread([corps, i]() { corps(i); });
        fils[i]->SetName("compteur");
    }
    for (int32 i = 0; i < kFils; ++i) fils[i]->Join();
    const float64 ms = chrono.Elapsed().milliseconds;
    for (int32 i = 0; i < kFils; ++i) delete fils[i];
    return ms;
}

static float64 Mediane(const float64 *v) {
    float64 a = v[0], b = v[1], c = v[2];
    if (a > b) { const float64 t = a; a = b; b = t; }
    if (b > c) { const float64 t = b; b = c; c = t; }
    if (a > b) { const float64 t = a; a = b; b = t; }
    return b;
}

struct Resultat {
    const char *nom;
    int64 total;
    float64 mediane;
};

int nkmain(const NkEntryState &) {
    const int64 attendu = static_cast<int64>(kFils) * kIncrements;
    float64 tNu[3], tMutex[3], tAtom[3], tLocal[3];
    int64 totNu = 0, totMutex = 0, totAtom = 0, totLocal = 0;

    for (int32 k = 0; k < 3; ++k) {
        g_nu = 0;
        tNu[k] = Mesurer([](int32) {
            for (int32 j = 0; j < kIncrements; ++j) g_nu += 1;
        });
        totNu = g_nu;

        g_mutexTotal = 0;
        tMutex[k] = Mesurer([](int32) {
            for (int32 j = 0; j < kIncrements; ++j) {
                NkScopedLock<NkMutex> verrou(g_mutex); // le verrou entoure l'INCREMENT, pas la boucle
                g_mutexTotal += 1;
            }
        });
        totMutex = g_mutexTotal;

        g_atomique.Store(0);
        tAtom[k] = Mesurer([](int32) {
            for (int32 j = 0; j < kIncrements; ++j) g_atomique.FetchAdd(1);
        });
        totAtom = static_cast<int64>(g_atomique.Load());

        tLocal[k] = Mesurer([](int32 i) {
            int64 local = 0; // propre a ce fil : rien n'est partage pendant le calcul
            for (int32 j = 0; j < kIncrements; ++j) local += 1;
            g_partiels[i].valeur = local;
        });
        totLocal = 0; // addition des huit totaux APRES les Join
        for (int32 i = 0; i < kFils; ++i) totLocal += g_partiels[i].valeur;
    }

    Resultat r[4] = {{"Sans protection", totNu, Mediane(tNu)},
                     {"Mutex", totMutex, Mediane(tMutex)},
                     {"Atomique", totAtom, Mediane(tAtom)},
                     {"Local", totLocal, Mediane(tLocal)}};
    for (int32 i = 0; i < 4; ++i)
        logger.Infof("[corrige] %s : total=%lld (attendu %lld), mediane=%.3f ms", r[i].nom,
                     static_cast<long long>(r[i].total), static_cast<long long>(attendu), r[i].mediane);

    // La moins chere parmi les versions JUSTES (la version sans protection est fausse).
    int32 meilleure = -1;
    for (int32 i = 1; i < 4; ++i)
        if (r[i].total == attendu && (meilleure < 0 || r[i].mediane < r[meilleure].mediane)) meilleure = i;

    FILE *out = std::fopen("prix.txt", "w");
    if (!out) return -1;
    for (int32 i = 0; i < 4; ++i)
        std::fprintf(out, "%s : total %lld (attendu %lld), temps médian %.3f ms\n\n", r[i].nom,
                     static_cast<long long>(r[i].total), static_cast<long long>(attendu), r[i].mediane);
    if (meilleure >= 0)
        std::fprintf(out, "La moins chère : %s (%.3f ms), parmi les versions qui donnent le bon total.\n\n", r[meilleure].nom,
                     r[meilleure].mediane);
    else
        std::fprintf(out, "La moins chère : <<A_COMPLETER: aucune version juste n'a donné le bon total, vérifier le code>>\n\n");
    std::fprintf(out,
                 "Pourquoi : Mutex (%.3f ms) : chaque incrément prend et rend le verrou, et les huit fils s'attendent les uns les autres : on paie l'attente du verrou. "
                 "Atomique (%.3f ms) : l'incrément est indivisible grâce au processeur, sans verrou ni attente passive, mais les huit cœurs se disputent la même case mémoire et le coût de cette coordination matérielle s'ajoute à chaque incrément. "
                 "Local (%.3f ms) : chaque fil compte dans sa propre variable, rien n'est partagé pendant le calcul, et seule l'addition finale des huit totaux se fait après les Join : c'est la seule version qui ne paie aucune coordination. "
                 "Sans protection (%.3f ms) est la plus rapide parce qu'elle ne coordonne rien, mais son total (%lld au lieu de %lld) est faux : ce n'est pas une option. "
                 "Le verrou de la version Mutex est pris autour de chaque incrément, pas autour de toute la boucle. "
                 "La sûreté vient de la façon de partager, pas du nombre de verrous.\n",
                 r[1].mediane, r[2].mediane, r[3].mediane, r[0].mediane, static_cast<long long>(r[0].total),
                 static_cast<long long>(attendu));
    std::fclose(out);
    logger.Info("[corrige] prix.txt ecrit");
    return 0;
}
