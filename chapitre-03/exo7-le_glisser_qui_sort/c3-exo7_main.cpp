#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "Chapitre03-Exo7";
    d.appVersion = "0.1.0";
    return d;
})());

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title    = "Exo7 - Glisser qui sort (C = bascule capture)";
    cfg.width    = 800;
    cfg.height   = 600;
    cfg.centered = true;

    NkWindow window;
    if (!window.Create(cfg)) return -1;

    bool captureActive    = false;
    bool enTrainDeGlisser = false;

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
            else if (auto* kp = ev->As<NkKeyPressEvent>()) {
                if (kp->GetKey() == NkKey::NK_C) {
                    captureActive = !captureActive;
                    window.CaptureMouse(captureActive);
                    std::printf("== Capture souris : %s ==\n", captureActive ? "ON" : "OFF");
                }
                else if (kp->GetKey() == NkKey::NK_ESCAPE) {
                    window.Close();
                }
            }
            else if (auto* press = ev->As<NkMouseButtonPressEvent>()) {
                if (press->IsLeft()) {
                    enTrainDeGlisser = true;
                    std::printf("Debut glisser a Teguis(%d,%d)\n", press->GetX(), press->GetY());
                }
            }
            else if (auto* release = ev->As<NkMouseButtonReleaseEvent>()) {
                if (release->IsLeft()) {
                    enTrainDeGlisser = false;
                    std::printf("Fin glisser\n");
                }
            }
            else if (auto* move = ev->As<NkMouseMoveEvent>()) {
                if (enTrainDeGlisser) {
                    std::printf("Glisser [%s] : teguis(%d,%d) ecran(%d,%d)\n",
                                captureActive ? "capture" : "libre",
                                move->GetX(), move->GetY(),
                                move->GetScreenX(), move->GetScreenY());
                }
            }
        }
    }
    return 0;
}
