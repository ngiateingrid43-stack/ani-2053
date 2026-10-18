#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKTime/NkTime.h"
#include "NKTime/NkChrono.h"
#include "NKLogger/NkLog.h"
#include "NKMath/NkColor.h"
#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Core/NkGraphicsApi.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKCanvas/Renderer/Core/NkRenderer2D.h"
#include "NKImage/Core/NkImage.h"
#include "NKThreading/NkThreadPool.h"
#include "NKThreading/NkFuture.h"
#include "NKCore/NkTraits.h"

#include <cstdio>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::renderer;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Futur";
    d.appVersion = "1.0.0";
    return d;
})());

// true : version Get (attend) ; false : version IsReady (demande a chaque trame).
static const bool kModeGet = false;

static long long TailleFichier(const char *chemin) {
    FILE *f = std::fopen(chemin, "rb");
    if (!f) return -1;
    std::fseek(f, 0, SEEK_END);
    const long long t = static_cast<long long>(std::ftell(f));
    std::fclose(f);
    return t;
}

int nkmain(const NkEntryState &state) {
    bool modeGet = kModeGet;
    const char *chemin = "grande-photo.png";
    for (usize i = 1; i < state.args.Size(); ++i) {
        const char *a = state.args[i].CStr();
        if (std::strcmp(a, "get") == 0) modeGet = true;
        else if (std::strcmp(a, "isready") == 0) modeGet = false;
        else chemin = a;
    }

    NkWindowConfig cfg;
    cfg.title = "Futur";
    cfg.width = 900;
    cfg.height = 400;
    cfg.centered = true;
    cfg.resizable = false;
    NkWindow window;
    if (!window.Create(cfg)) {
        logger.Error("[futur] fenetre impossible");
        return -1;
    }
    NkContextDesc desc; // backend par defaut
    NkRenderWindow target(window, desc);
    if (!target.IsValid()) {
        window.Close();
        return -2;
    }

    bool running = true;
    auto &events = NkEvents();
    events.AddEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent *) { running = false; });

    // La promesse doit vivre jusqu'a ce que la tache l'ait remplie : elle est declaree ici,
    // dans nkmain, et la reserve (declaree apres) est detruite avant elle.
    NkPromise<NkImage> promesse;
    NkFuture<NkImage> futur = promesse.GetFuture();
    NkThreadPool pool;

    NkImage image;
    float64 tempsChargementMs = 0.0; // ecrit par la tache, lu apres que le futur est pret
    bool lance = false, pret = false;
    int32 trame = 0, trameDansChargement = 0, tramesApresPret = 0;
    float64 trameMax = 0.0;
    float32 x = 0.f;

    while (running && window.IsOpen()) {
        NkChrono chronoTrame;
        while (NkEvent *ev = events.PollEvent()) { (void)ev; }
        if (!running) break;

        bool dansFenetre = false; // vrai de la trame de lancement jusqu'a la trame ou le resultat est pris
        if (!lance && trame == 60) {
            lance = true;
            dansFenetre = true;
            pool.Enqueue([&promesse, &tempsChargementMs, chemin]() {
                NkChrono c;
                NkImage img; // la tache charge des pixels, elle ne dessine pas
                if (!img.Load(chemin)) logger.Errorf("[futur] chargement impossible : %s", chemin);
                tempsChargementMs = c.Elapsed().milliseconds;
                promesse.SetValue(nkentseu::traits::NkMove(img));
            });
            if (!modeGet) ++trameDansChargement; // la trame de lancement est dessinee pendant le chargement
            if (modeGet) {
                // Version Get : on attend ICI, la boucle ne dessine plus pendant le chargement.
                image = nkentseu::traits::NkMove(futur.Get());
                pret = true;
            }
        } else if (lance && !pret) {
            dansFenetre = true;
            // Version IsReady : on demande, on ne bloque jamais.
            if (futur.IsReady()) {
                image = nkentseu::traits::NkMove(futur.Get());
                pret = true;
            } else {
                ++trameDansChargement;
            }
        }

        // Dessin : seule cette boucle dessine.
        const math::NkVec2u sz = target.GetSize();
        const float32 W = static_cast<float32>(sz.x);
        target.Clear(NkColor2D{18, 20, 28, 255});
        NkRenderer2D &r = target.GetRenderer2D();
        r.DrawFilledRect(NkRect2f{x, 180.f, 40.f, 40.f},
                         pret ? NkColor2D{90, 220, 120, 255} : NkColor2D{255, 180, 60, 255});
        target.Display();
        x += 1.f;
        if (x > W) x = 0.f;

        // Duree de la trame (du debut de la boucle a l'affichage), retenue pendant le chargement.
        const float64 ms = chronoTrame.Elapsed().milliseconds;
        if (dansFenetre && ms > trameMax) trameMax = ms;
        ++trame;
        if (pret && ++tramesApresPret > 60) running = false; // on laisse tourner 60 trames de plus
    }
    pool.Join();

    const long long fichier = TailleFichier(chemin);
    const long long decode = static_cast<long long>(image.Width()) * static_cast<long long>(image.Height()) *
                             static_cast<long long>(image.BytesPP());
    const char *mode = modeGet ? "Get" : "IsReady";
    logger.Infof("[futur] image %dx%d, %lld octets decodes (fichier %lld), chargement %.3f ms",
                 static_cast<int>(image.Width()), static_cast<int>(image.Height()), decode, fichier, tempsChargementMs);
    logger.Infof("[futur] %s : trame la plus longue %.3f ms, %d trame(s) dessinee(s) pendant le chargement", mode,
                 trameMax, static_cast<int>(trameDansChargement));

    FILE *out = std::fopen(modeGet ? "cadence-get.txt" : "cadence-isready.txt", "w");
    if (out) {
        std::fprintf(out, "Image : %dx%d pixels, %lld octets décodés (fichier de %lld octets), chargement en %.3f ms\n",
                     static_cast<int>(image.Width()), static_cast<int>(image.Height()), decode, fichier,
                     tempsChargementMs);
        std::fprintf(out, "%s : trame la plus longue %.3f ms, %d trames pendant le chargement\n", mode, trameMax,
                     static_cast<int>(trameDansChargement));
        std::fclose(out);
    }
    return 0;
}
