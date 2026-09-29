// c4-exo2 : la lettre (ce que produit la touche) et la position (code physique).
// Position = NkKeyPressEvent::GetKey() (physique, US-QWERTY) ; lettre = NkTextInputEvent::GetUtf8() (selon la disposition).
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo2";
    d.appVersion = "1.0.0";
    return d;
})());

int nkmain(const NkEntryState& state) {
    (void)state;
    NkWindowConfig cfg;
    cfg.title = "c4-exo2 - tapez des touches (Echap = quitter)"; cfg.width = 700; cfg.height = 300; cfg.centered = true;
    NkWindow window;
    if (!window.Create(cfg)) return -1;
    std::printf("Chaque touche affiche : POSITION (code NkKey) puis LETTRE produite.\n");

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            } else if (auto* kp = ev->As<NkKeyPressEvent>()) {
                std::printf("position : NkKey=%d  (Ctrl=%d Shift=%d Alt=%d AltGr=%d)\n",
                            (int)kp->GetKey(), (int)kp->HasCtrl(), (int)kp->HasShift(), (int)kp->HasAlt(), (int)kp->HasAltGr());
                if (kp->GetKey() == NkKey::NK_ESCAPE) window.Close();
            } else if (auto* txt = ev->As<NkTextInputEvent>()) {
                if (txt->IsPrintable())
                    std::printf("   lettre  : \"%s\"  (codepoint U+%04X)\n", txt->GetUtf8(), (unsigned)txt->GetCodepoint());
            }
        }
        NkChrono::Sleep((int64)16);
    }
    return 0;
}
