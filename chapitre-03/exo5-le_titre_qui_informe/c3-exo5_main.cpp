#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "Chapitre03-Exo5";
    d.appVersion = "0.1.0";
    return d;
})());

static void MettreAJourTitre(NkWindow& window, const char* nomDocument, bool modifie) {
    const math::NkVec2u sz = window.GetSize();
    char titre[256];
    std::snprintf(titre, sizeof(titre), "%s%s - %ux%u",
                  nomDocument, modifie ? "*" : "", sz.x, sz.y);
    window.SetTitle(titre);
}

int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title    = "Exo5";
    cfg.width    = 1280;
    cfg.height   = 720;
    cfg.centered = true;

    NkWindow window;
    if (!window.Create(cfg)) return -1;

    const char* nomDocument = "sansnom.txt";
    bool modifie = false;
    MettreAJourTitre(window, nomDocument, modifie);   // etat initial, une seule fois

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
            else if (ev->Is<NkWindowResizeEvent>()) {
                // La taille a change : on rafraichit le titre, rien de plus.
                MettreAJourTitre(window, nomDocument, modifie);
            }
            else if (auto* kp = ev->As<NkKeyPressEvent>()) {
                if (kp->GetKey() == NkKey::NK_F2) {
                    modifie = !modifie;                 // simule une modification
                    MettreAJourTitre(window, nomDocument, modifie);
                }
                else if (kp->GetKey() == NkKey::NK_ESCAPE) {
                    window.Close();
                }
            }
        }
    }
    return 0;
}
