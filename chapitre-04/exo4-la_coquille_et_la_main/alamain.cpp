// Version a la main : on assemble soi-meme fenetre, cible, horloge et boucle.
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKTime/NkChrono.h"
#include "NKTime/NkClock.h"
#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo4-alamain";
    d.appVersion = "1.0.0";
    return d;
})());

namespace {
constexpr float32 LARGEUR = 800.f;
constexpr float32 HAUTEUR = 450.f;
constexpr float32 COTE    = 50.f;
constexpr float32 VITESSE = 100.f; // pixels par seconde
}

int nkmain(const NkEntryState &state) {
    (void)state;

    NkWindowConfig cfg;
    cfg.title     = "c4-exo4 - a la main";
    cfg.width     = static_cast<uint32>(LARGEUR);
    cfg.height    = static_cast<uint32>(HAUTEUR);
    cfg.centered  = true;
    cfg.resizable = false;

    NkWindow window;
    if (!window.Create(cfg)) return -1;
    if (!window.IsOpen()) return -2;             // verification 1 : la fenetre est ouverte

    // Le moteur de rendu se choisit ici, a la main.
    NkContextDesc desc;
#if defined(NKENTSEU_PLATFORM_WINDOWS)
    desc.api = NkGraphicsApi::NK_GFX_API_DX11;
#else
    desc.api = NkGraphicsApi::NK_GFX_API_OPENGL;
#endif
    NkRenderWindow target(window, desc);
    if (!target.IsValid()) return -3;            // verification 2 : le contexte de rendu existe

    NkClock clock;
    float32 x = 0.f;
    while (window.IsOpen()) {
        const float32 dt = clock.Tick().delta;   // 1. le temps ecoule

        while (NkEvent *ev = NkEvents().PollEvent()) {   // 2. les evenements
            if (ev->Is<NkWindowCloseEvent>()) window.Close();
        }

        x += VITESSE * dt;                       // 3. la mise a jour
        if (x > LARGEUR) x = -COTE;

        target.Clear(NkColor2D{18, 18, 24, 255});        // 4. Clear
        target.GetRenderer2D().DrawFilledRect({x, (HAUTEUR - COTE) * 0.5f, COTE, COTE}, NkColor2D::Red);  // 5. dessin
        target.Display();                                // 6. Display

        NkChrono::Sleep(static_cast<int64>(16));
    }
    return 0;
}