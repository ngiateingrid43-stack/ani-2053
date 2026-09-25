# Exercice 2 

## 1. cfg.resizable = false

 **1.1 CODE**

    ```
    #include "NKWindow/NKWindow.h"
    #include "NKWindow/NKMain.h"

    using namespace nkentseu;

    int nkmain(const NkEntryState& state) {
        // 1) Décrire la fenêtre
        NkWindowConfig cfg;
        cfg.title  = "Hello NKWindow";
        cfg.width  = 1280;
        cfg.height = 720;

        // 1.1) Interdire le redimensionnement
        cfg.resizable = false;

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
            // mettre à jour la logique, puis dessiner (NKCanvas)
        }

        return 0;
    }
    ```
 **1.2 Remaques**

    ``
    la fenetre garde une taille que l'on  peut p modifier le redimentionnement n'est blocque pas
    rien ne change
    ``

## 2. cfg.movable = false

**2.1 CODE**

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;

    // 1.1) Interdire le redimensionnement
    cfg.movable = false;

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
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}
```

 **2.2 Remarque**

 ``
 on constate que la fenetre est deplacable on n'a blocque pas le deplacement de la fentre
 rien ne change
 ``

## 3. cfg.closable = false

**3.1 CODE**

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;

    // 1.1) Interdire le redimensionnement
    cfg.closable = false;

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
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}
```

 **3.2 Remarque**

 ``
rien ne change
 ``

## 4. cfg.minimizable = false

**4.1 CODE**

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;

    // 1.1) Interdire le redimensionnement
    cfg.minimizable = false;

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
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}
```

 **4.2 Remarque**

 ``
 rien ne change
 ``

 ## 5. cfg.maximizable = false

**5.1 CODE**

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;

    // 1.1) Interdire le redimensionnement
    cfg.maximizable = false;

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
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}
```

 **5.2 Remarque**

 ``
 rien ne change
 ``

## 6. cfg.canFullscreen = false

**6.1 CODE**

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;

    // 1.1) Interdire le redimensionnement
    cfg.canFullscreen = false;

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
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}
```

 **6.2 Remarque**

 ``
 rien ne change
 ``

## 7. cfg.fullscreen = false

**7.1 CODE**

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1) Décrire la fenêtre
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;

    // 1.1) Interdire le redimensionnement
    cfg.fullscreen = false;

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
        // mettre à jour la logique, puis dessiner (NKCanvas)
    }

    return 0;
}
```

 **7.2 Remarque**

 ``
 rien ne change
 ``





