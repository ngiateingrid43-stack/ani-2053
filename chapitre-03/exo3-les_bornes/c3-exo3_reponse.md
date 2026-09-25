# Exercice 3

## 1. Fixation d'une taille minimale

**1.1 CODE**

```
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

    //Fixation d'une taille minimale
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
```

**1.2 Sortie Obtenue dans le terminale**

``
Taille de la fen├¬tre : 624 x 441
``
``taille minimalle obtenu suite au plus petit redimentionnement de ma fenetre cree alors que la taille minimale fixee par moi est 640 x 480``

## 2. SaNS taille minimale fixer

**2.1 CODE**

```
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
```

**2.2 Sortie du terminale**

``
Taille de la fen├¬tre : 144 x 51
``
``taille minimalle obtenu suite au plus petit redimentionnement de ma fenetre cree alors que je n'ais pas fixee de limite de taille``

