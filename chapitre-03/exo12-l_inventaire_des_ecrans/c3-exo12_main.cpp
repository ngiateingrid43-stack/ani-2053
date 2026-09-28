#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "Chapitre03-Exo12";
    d.appVersion = "0.1.0";
    return d;
})());

static void AfficherEcrans(NkWindow& window) {
    NkVector<NkDisplayInfo> ecrans = window.EnumerateMonitors();
    std::printf("--- %u ecran(s) detecte(s) ---\n", (uint32)ecrans.Size());
    for (usize i = 0; i < ecrans.Size(); ++i) {
        const NkDisplayInfo& m = ecrans[i];
        std::printf("Ecran %u : %ux%u @ %uHz, echelle x%.2f, pos(%d,%d), primaire=%s, nom=%s\n",
                    m.index, m.width, m.height, m.refreshRate, m.dpiScale,
                    m.posX, m.posY, m.isPrimary ? "oui" : "non", m.name);
    }
    const NkDisplayInfo cour = window.GetCurrentMonitor();
    std::printf(">>> La fenetre est actuellement sur l'ecran %u (%s)\n\n",
                cour.index, cour.name);
}

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title    = "Exo12 - Inventaire des ecrans";
    cfg.width    = 900;
    cfg.height   = 500;
    cfg.centered = true;

    NkWindow window;
    if (!window.Create(cfg)) return -1;

    AfficherEcrans(window);   // etat initial

    math::NkVec2u dernierePos = window.GetPosition();

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
        }

        // On ne reverifie l'ecran courant que si la position a change
        // (deplacement possible d'un ecran a l'autre).
        const math::NkVec2u posActuelle = window.GetPosition();
        if (posActuelle.x != dernierePos.x || posActuelle.y != dernierePos.y) {
            dernierePos = posActuelle;
            AfficherEcrans(window);
        }
    }
    return 0;
}
