#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"
#include "NKThreading/NkThread.h"
#include "NKThreading/NkThreadPool.h"

#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::threading; // NkThread, NkThreadPool, NkMutex... vivent dans nkentseu::threading

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Reserve";
    d.appVersion = "1.0.0";
    return d;
})());

static const int32 kNbTaches = 200;
static const int32 kNbEntiers = 1000;

static int32 g_donnees[kNbEntiers];
static int64 g_resultats[kNbTaches]; // un emplacement par tache : aucune ecriture partagee

// La tache : additionner mille entiers dans une variable locale, 'rep' fois.
// Le resultat est range puis affiche : le compilateur ne peut pas retirer le calcul.
static int64 Tache(int32 rep) {
    int64 somme = 0;
    for (int32 r = 0; r < rep; ++r)
        for (int32 i = 0; i < kNbEntiers; ++i)
            somme += static_cast<int64>(g_donnees[i]) * (r + 1);
    return somme;
}

static int64 SommeResultats() {
    int64 s = 0;
    for (int32 t = 0; t < kNbTaches; ++t) s += g_resultats[t];
    return s;
}

// Version reserve : 200 Enqueue, puis Join. La reserve est creee AVANT le chronometre.
static float64 MesurerReserve(int32 rep, int32 *ouvriers) {
    NkThreadPool pool;
    *ouvriers = static_cast<int32>(pool.GetNumWorkers());
    NkChrono chrono;
    for (int32 t = 0; t < kNbTaches; ++t)
        pool.Enqueue([t, rep]() { g_resultats[t] = Tache(rep); });
    pool.Join();
    return chrono.Elapsed().milliseconds;
}

// Version fils : un NkThread par tache, 200 Join.
static float64 MesurerFils(int32 rep) {
    NkThread *fils[kNbTaches];
    NkChrono chrono;
    for (int32 t = 0; t < kNbTaches; ++t) {
        fils[t] = new NkThread([t, rep](void *) { g_resultats[t] = Tache(rep); });
        fils[t]->SetName("tache");
    }
    for (int32 t = 0; t < kNbTaches; ++t) fils[t]->Join(); // chaque fil cree est joint
    const float64 ms = chrono.Elapsed().milliseconds;
    for (int32 t = 0; t < kNbTaches; ++t) delete fils[t];
    return ms;
}

static float64 Mediane(const float64 *v) {
    float64 a = v[0], b = v[1], c = v[2];
    if (a > b) { const float64 t = a; a = b; b = t; }
    if (b > c) { const float64 t = b; b = c; c = t; }
    if (a > b) { const float64 t = a; a = b; b = t; }
    return b;
}

int nkmain(const NkEntryState &) {
    for (int32 i = 0; i < kNbEntiers; ++i) g_donnees[i] = (i * 7 + 3) % 1000;
    const int32 kLongue = 100; // tache longue : 100 fois plus de travail

    float64 reserve[3], fils[3], reserveL[3], filsL[3];
    int32 ouvriers = 0;
    int64 controle = 0;

    for (int32 k = 0; k < 3; ++k) {
        reserve[k] = MesurerReserve(1, &ouvriers);
        controle += SommeResultats();
        fils[k] = MesurerFils(1);
        controle += SommeResultats();
    }
    for (int32 k = 0; k < 3; ++k) {
        reserveL[k] = MesurerReserve(kLongue, &ouvriers);
        controle += SommeResultats();
        filsL[k] = MesurerFils(kLongue);
        controle += SommeResultats();
    }
    logger.Infof("[reserve] controle (somme des resultats) = %lld", static_cast<long long>(controle));
    logger.Infof("[reserve] ouvriers = %d", static_cast<int>(ouvriers));
    for (int32 k = 0; k < 3; ++k)
        logger.Infof("[reserve] courte %d : reserve=%.3f ms, fils=%.3f ms", static_cast<int>(k + 1), reserve[k], fils[k]);
    for (int32 k = 0; k < 3; ++k)
        logger.Infof("[reserve] longue %d : reserve=%.3f ms, fils=%.3f ms", static_cast<int>(k + 1), reserveL[k], filsL[k]);

    const float64 mR = Mediane(reserve), mF = Mediane(fils), mRL = Mediane(reserveL), mFL = Mediane(filsL);
    const float64 rapportCourt = (mR > 0.0) ? mF / mR : 0.0;
    const float64 rapportLong = (mRL > 0.0) ? mFL / mRL : 0.0;

    FILE *out = std::fopen("reserve.txt", "w");
    if (!out) {
        logger.Error("[reserve] impossible d'ecrire reserve.txt");
        return -1;
    }
    std::fprintf(out,
                 "Tâche : additionner 1000 entiers d'un tableau dans une variable locale, 200 fois (tâche courte, de l'ordre de la microseconde) ; la tâche longue répète cette addition 100 fois de plus. Résultat de contrôle affiché : %lld.\n\n",
                 static_cast<long long>(controle));
    std::fprintf(out, "Réserve : %.3f ms, %.3f ms, %.3f ms\n\n", reserve[0], reserve[1], reserve[2]);
    std::fprintf(out, "Fils : %.3f ms, %.3f ms, %.3f ms\n\n", fils[0], fils[1], fils[2]);
    std::fprintf(out, "Ouvriers : %d\n\n", static_cast<int>(ouvriers));
    std::fprintf(out, "Tâche longue : réserve %.3f ms, fils %.3f ms (médianes de trois mesures)\n\n", mRL, mFL);

    std::fprintf(out, "Pourquoi : Créer un fil coûte cher : pour chacun, le système réserve une pile, l'ordonnance, puis le détruit. ");
    std::fprintf(out, "La version Fils paie ce coût 200 fois ; la réserve crée ses %d ouvriers une seule fois et leur distribue les tâches. ",
                 static_cast<int>(ouvriers));
    if (mF > mR) {
        std::fprintf(out, "Pour la tâche courte, ce coût de création domine le calcul : %.3f ms pour la réserve contre %.3f ms pour les fils (rapport %.1f). ",
                     mR, mF, rapportCourt);
    } else {
        std::fprintf(out, "Pour la tâche courte, la réserve n'a pas été plus rapide dans mes mesures (%.3f ms contre %.3f ms pour les fils) : <<A_COMPLETER: expliquer ce que vous voyez, par exemple le coût de la file de tâches>>. ",
                     mR, mF);
    }
    if (rapportLong < rapportCourt) {
        std::fprintf(out, "Avec la tâche cent fois plus longue, le calcul domine à son tour et l'écart relatif se réduit : %.3f ms contre %.3f ms (rapport %.1f), car le coût fixe de création d'un fil est amorti par un travail plus long.\n",
                     mRL, mFL, rapportLong);
    } else {
        std::fprintf(out, "Avec la tâche cent fois plus longue, l'écart relatif ne se réduit pas dans mes mesures (%.3f ms contre %.3f ms, rapport %.1f) : <<A_COMPLETER: commenter>>.\n",
                     mRL, mFL, rapportLong);
    }
    std::fclose(out);
    logger.Info("[reserve] reserve.txt ecrit");
    return 0;
}
