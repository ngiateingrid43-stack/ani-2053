#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKTime/NkClock.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo1";
    d.appVersion = "1.0.0";
    return d;
})());

namespace {

const char* Famille(NkEvent* ev) {
    if (ev->HasCategory(NkEventCategory::NK_CAT_KEYBOARD)) return "clavier";
    if (ev->HasCategory(NkEventCategory::NK_CAT_MOUSE))    return "souris";
    if (ev->HasCategory(NkEventCategory::NK_CAT_GAMEPAD))  return "manette";
    if (ev->HasCategory(NkEventCategory::NK_CAT_TOUCH))    return "tactile";
    return "autre (fenetre/systeme/depot...)";
}

} // namespace

int nkmain(const NkEntryState& state) {
    (void)state;
    NkWindowConfig cfg;
    cfg.title = "c4-exo1 - journal des evenements"; cfg.width = 800; cfg.height = 450; cfg.centered = true;
    cfg.dropEnabled = true;                                // pour recevoir les depots de fichiers
    NkWindow window;
    if (!window.Create(cfg)) return -1;

    NkClock clock;
    float32 t = 0.f;
    uint32 total = 0, cetteSeconde = 0, maxSeconde = 0;

    while (window.IsOpen()) {
        t += clock.Tick().delta;
        while (NkEvent* ev = NkEvents().PollEvent()) {
            ++total; ++cetteSeconde;
            std::printf("[%llu ms] famille=%s type=%d | %s\n",
                        (unsigned long long)ev->GetTimestamp(), Famille(ev), (int)ev->GetType(), ev->ToString().CStr());
            if (ev->Is<NkWindowCloseEvent>()) window.Close();
        }
        if (t >= 1.f) {                                    // bilan chaque seconde
            if (cetteSeconde > maxSeconde) maxSeconde = cetteSeconde;
            std::printf("=== %u evenements pendant la derniere seconde (max %u, total %u) ===\n", cetteSeconde, maxSeconde, total);
            cetteSeconde = 0; t -= 1.f;
        }
        NkChrono::Sleep((int64)16);
    }
    return 0;
}
