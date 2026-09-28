#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "Chapitre03-Exo10";
    d.appVersion = "0.1.0";
    return d;
})());

static const int32 kBarHeight   = 32;
static const int32 kButtonWidth = 46;

enum class HitZone { None, Drag, Minimize, MaximizeRestore, Close };

static HitZone HitTest(const math::NkVec2u& winSize, int32 x, int32 y) {
    if (y < 0 || y >= kBarHeight) {
        return HitZone::None;   // en dehors de la barre de titre custom
    }
    const int32 w = (int32)winSize.x;
    if (x >= w - kButtonWidth) {
        return HitZone::Close;
    }
    if (x >= w - 2 * kButtonWidth) {
        return HitZone::MaximizeRestore;
    }
    if (x >= w - 3 * kButtonWidth) {
        return HitZone::Minimize;
    }
    return HitZone::Drag;
}

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title     = "Exo10 - barre de titre custom";
    cfg.width     = 960;
    cfg.height    = 600;
    cfg.centered  = true;
    cfg.frame     = false;   // pas de bordure/barre de titre systeme
    cfg.resizable = true;

    NkWindow window;
    if (!window.Create(cfg)) {
        return -1;
    }

    std::printf("Fenetre sans bordure : bande du haut (32px) = barre custom.\n");
    std::printf("Les 46 derniers px de cette bande, de droite a gauche : Fermer, Agrandir/Restaurer, Reduire.\n");
    std::printf("Le reste de la bande : glisser pour deplacer, double-clic pour agrandir/restaurer.\n");

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
            else if (auto* press = ev->As<NkMouseButtonPressEvent>()) {
                if (press->IsLeft()) {
                    const HitZone zone = HitTest(window.GetSize(), press->GetX(), press->GetY());
                    switch (zone) {
                        case HitZone::Drag:
                            window.BeginDragMove();   // hand-off natif du deplacement
                            std::printf("[barre] deplacement demarre.\n");
                            break;
                        case HitZone::Minimize:
                            window.Minimize();
                            std::printf("[bouton] reduire.\n");
                            break;
                        case HitZone::MaximizeRestore:
                            if (window.IsMaximized()) window.Restore();
                            else                        window.Maximize();
                            std::printf("[bouton] agrandir/restaurer -> maximisee=%d\n", (int)window.IsMaximized());
                            break;
                        case HitZone::Close:
                            std::printf("[bouton] fermer.\n");
                            window.Close();
                            break;
                        default:
                            break;
                    }
                }
            }
            else if (auto* dbl = ev->As<NkMouseDoubleClickEvent>()) {
                if (dbl->IsLeft() && HitTest(window.GetSize(), dbl->GetX(), dbl->GetY()) == HitZone::Drag) {
                    if (window.IsMaximized()) window.Restore();
                    else                        window.Maximize();
                    std::printf("[double-clic] agrandir/restaurer -> maximisee=%d\n", (int)window.IsMaximized());
                }
            }
        }

        NkChrono::Sleep((int64)8);
    }

    return 0;
}
