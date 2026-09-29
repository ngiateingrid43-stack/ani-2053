#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo3";
    d.appVersion = "1.0.0";
    return d;
})());

namespace {

int gNbFermetures = 0;

void TraiterFermeture(NkWindow& window, const char* origine) {
    ++gNbFermetures;
    std::printf("fermeture demandee (origine : %s) -> chemin unique, appel n.%d\n", origine, gNbFermetures);
    window.Close();
}

} // namespace

int nkmain(const NkEntryState& state) {
    (void)state;
    NkWindowConfig cfg;
    cfg.title = "c4-exo3 - fermez par la croix, Alt+F4 ou la touche Q"; cfg.width = 700; cfg.height = 300; cfg.centered = true;
    NkWindow window;
    if (!window.Create(cfg)) return -1;

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                TraiterFermeture(window, "evenement de fermeture (croix / Alt+F4 / gestionnaire)");
            } else if (auto* kp = ev->As<NkKeyPressEvent>()) {
                if (kp->GetKey() == NkKey::NK_Q) TraiterFermeture(window, "ma touche Q");   // meme fonction
            }
        }
        NkChrono::Sleep((int64)16);
    }
    std::printf("boucle terminee proprement (%d appel(s) au chemin de fermeture)\n", gNbFermetures);
    return 0;
}
