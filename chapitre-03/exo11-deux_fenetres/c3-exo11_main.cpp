#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName           = "Chapitre03-Exo11";
    d.appVersion        = "0.1.0";
    d.enableMultiWindow  = true;
    return d;
})());

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfgA;
    cfgA.title    = "Fenetre A";
    cfgA.width    = 640;
    cfgA.height   = 480;
    cfgA.centered = false;
    cfgA.x = 80;  cfgA.y = 120;

    NkWindowConfig cfgB;
    cfgB.title    = "Fenetre B";
    cfgB.width    = 640;
    cfgB.height   = 480;
    cfgB.centered = false;
    cfgB.x = 760; cfgB.y = 120;

    NkWindow fenetreA, fenetreB;
    if (!fenetreA.Create(cfgA)) return -1;
    if (!fenetreB.Create(cfgB)) return -1;

    const NkWindowId idA = fenetreA.GetId();
    const NkWindowId idB = fenetreB.GetId();

    while (fenetreA.IsOpen() || fenetreB.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (auto* press = ev->As<NkMouseButtonPressEvent>()) {
                const NkWindowId src = ev->GetWindowId();
                if (src == idA)      std::printf("Clic recu par Fenetre A\n");
                else if (src == idB) std::printf("Clic recu par Fenetre B\n");
            }
            else if (ev->Is<NkWindowCloseEvent>()) {
                const NkWindowId src = ev->GetWindowId();
                if (src == idA)      fenetreA.Close();
                else if (src == idB) fenetreB.Close();
            }
        }
    }
    return 0;
}
