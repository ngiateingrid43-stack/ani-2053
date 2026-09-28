#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "Chapitre03-Exo4";
    d.appVersion = "0.1.0";
    return d;
})());

static void RefreshTitle(NkWindow& window) {
    const math::NkVec2u winSize = window.GetSize();          // taille FENETRE (client, logique)
    const NkSurfaceDesc surf    = window.GetSurfaceDesc();   // taille CIBLE DE RENDU (surface, physique)
    const float32 dpi           = window.GetDpiScale();      // facteur d'echelle

    char buf[192];
    std::snprintf(buf, sizeof(buf),
                   "Exo4 | fenetre %ux%u | cible de rendu %ux%u | echelle x%.2f",
                   winSize.x, winSize.y, surf.width, surf.height, (double)dpi);
    window.SetTitle(buf);

    std::printf("fenetre=%ux%u  cible de rendu=%ux%u  echelle=x%.2f\n",
                winSize.x, winSize.y, surf.width, surf.height, (double)dpi);
}

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title    = "Exo4";
    cfg.width    = 960;
    cfg.height   = 540;
    cfg.centered = true;

    NkWindow window;
    if (!window.Create(cfg)) {
        return -1;
    }

    math::NkVec2u lastSize    = window.GetSize();
    math::NkVec2u lastSurface = { window.GetSurfaceDesc().width, window.GetSurfaceDesc().height };
    float32       lastDpi     = window.GetDpiScale();
    RefreshTitle(window);

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
        }

        const math::NkVec2u curSize    = window.GetSize();
        const NkSurfaceDesc curSurfD   = window.GetSurfaceDesc();
        const math::NkVec2u curSurface = { curSurfD.width, curSurfD.height };
        const float32       curDpi     = window.GetDpiScale();

        if (curSize.x != lastSize.x || curSize.y != lastSize.y ||
            curSurface.x != lastSurface.x || curSurface.y != lastSurface.y ||
            curDpi != lastDpi) {
            lastSize    = curSize;
            lastSurface = curSurface;
            lastDpi     = curDpi;
            RefreshTitle(window);
        }

        NkChrono::Sleep((int64)8);
    }

    return 0;
}