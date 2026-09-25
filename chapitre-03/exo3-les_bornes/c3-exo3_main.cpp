#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include <iostream>

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;
    cfg.minWidth  = 640;   // largeur minimale
    cfg.minHeight = 480;   // hauteur minimale


    // 2) Créer la fenêtre
    NkWindow window;
    if (!window.Create(cfg)) {
        return -1;   // échec de création
    }

    // 3) Boucle principale (voir §3)
    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            // traiter les entrées — détaillé dans le guide NKEvent
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();          // l'utilisateur veut fermer
            }
        }
        std::cout << "Taille de la fenêtre : " << window.GetSize().x << " x " << window.GetSize().y << std::endl;
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}