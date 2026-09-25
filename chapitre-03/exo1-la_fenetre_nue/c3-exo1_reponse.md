# Exercice 1

## Nombre de ligne de ce programme

```
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
using namespace nkentseu;
int nkmain(const NkEntryState& state) {
    NkWindowConfig cfg;
    cfg.title  = "Hello NKWindow";
    cfg.width  = 1280;
    cfg.height = 720;
    NkWindow window;
    if (!window.Create(cfg)) {
        return -1;   // échec de création
    }
    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();          
            }
        }
    }
    return 0;
}
```
**Ce programme compte 20 lignes**



