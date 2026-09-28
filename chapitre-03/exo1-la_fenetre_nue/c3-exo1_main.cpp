#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
using namespace nkentseu;
int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg; // 1) Décrire la fenêtre
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;
    NkWindow window; // 2) Créer la fenêtre
    if (!window.Create(cfg)) {
        return -1;   // échec de création
    }
    while (window.IsOpen()) { // 3) Boucle principale (voir §3)
        while (NkEvent* ev = NkEvents().PollEvent()) { // traiter les entrées — détaillé dans le guide NKEvent      
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();          // l'utilisateur veut fermer
            }
        }// mettre à jour la logique, puis dessiner (NKCanvas)
    }
    return 0;
}