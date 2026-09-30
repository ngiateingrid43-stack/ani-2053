#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo5";
    d.appVersion = "1.0.0";
    return d;
})());

namespace {
constexpr float32 PANNEAU_X = 0.f, PANNEAU_Y = 0.f, PANNEAU_W = 220.f, PANNEAU_H = 160.f;
bool DansPanneau(float32 x, float32 y) {
    return x >= PANNEAU_X && x < PANNEAU_X + PANNEAU_W && y >= PANNEAU_Y && y < PANNEAU_Y + PANNEAU_H;
}
}

int nkmain(const NkEntryState& state) {
    (void)state;
    NkWindowConfig cfg;
    cfg.title = "c4-exo5 - cliquez dans / hors du coin haut-gauche (220x160)"; cfg.width = 800; cfg.height = 500; cfg.centered = true;
    NkWindow window;
    if (!window.Create(cfg)) return -1;

    auto& events = NkEvents();
    // Ordre d'inscription = ordre d'appel : le panneau (au-dessus) passe AVANT la scene (dessous).
    events.AddEventCallback<NkMouseButtonPressEvent>([](NkMouseButtonPressEvent* e) {
        if (DansPanneau((float32)e->GetX(), (float32)e->GetY())) {
            std::printf("[panneau] clic (%d,%d) CONSOMME\n", (int)e->GetX(), (int)e->GetY());
            e->MarkHandled();
        }
    });
    events.AddEventCallback<NkMouseButtonPressEvent>([](NkMouseButtonPressEvent* e) {
        if (e->IsHandled()) {                       // filet : si le systeme livre quand meme, on le voit ici
            std::printf("[scene]   clic deja consomme, ignore (le systeme l'a quand meme livre)\n");
            return;
        }
        std::printf("[scene]   clic (%d,%d) recu\n", (int)e->GetX(), (int)e->GetY());
    });

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {   // pompe la file : declenche aussi les callbacks
            if (ev->Is<NkWindowCloseEvent>()) window.Close();
        }
        NkChrono::Sleep((int64)16);
    }
    return 0;
}
