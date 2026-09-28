# Exercice 1

## 1. Nombre de ligne de ce programme

```
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
```
**Ce programme compte 21 lignes**

## 2. Correspondance avec la code du cHapitre

#	Code source (actuel)	Code du chapitre	Remarque

| # | Code source (actuel) | Code du chapitre | Remarque |
|:-:|----------------------|------------------|----------|
| 1 | `#include "NKWindow/NKWindow.h"` | `#include "NKWindow/NKWindow.h"` | Identique |
| 2 | `#include "NKWindow/NKMain.h"` | `#include "NKWindow/NKMain.h"` | Identique |
| 3 | *(ligne vide)* | *(ligne vide)* | Identique |
| 4 | `using namespace nkentseu;` | *(absent)* | Pas de `using` dans le chapitre |
| 5 | `int nkmain(const NkEntryState& state) {` | `int nkmain(const NkEntryState &state) {` | Style d'espacement `&` |
| 6 | `NkWindowConfig cfg; // 1) Décrire la fenêtre` | `NkWindowConfig cfg;` | Commentaire en plus |
| 7 | `cfg.title  = "Hello NKWindow";` | `cfg.title  = "Ma fenetre";` | Titre différent |
| 8 | `cfg.width  = 1280;` | `cfg.width  = 1280;` | Identique |
| 9 | `cfg.height = 720;` | `cfg.height = 720;` | Identique |
| 10 | `NkWindow window; // 2) Créer la fenêtre` | *(ligne vide)* | Ligne absente dans le chapitre |
| 11 | `if (!window.Create(cfg)) {` | `NkWindow window(cfg);` | Constructeur vs `Create()` |
| 12 | `return -1;   // échec de création` | `if (!window.IsOpen()) {` | Vérification différente |
| 13 | `}` | `logger.Error("[app] creation fenetre echouee");` | Log d'erreur |
| 14 | `while (window.IsOpen()) { // 3) Boucle principale (voir §3)` | `return -1;` | Retour d'erreur |
| 15 | `while (NkEvent* ev = NkEvents().PollEvent()) {` | `}` | Fermeture du `if` |
| 16 | `if (ev->Is<NkWindowCloseEvent>()) {` | `while (window.IsOpen()) { /* les evenements arrivent ici */ }` | Boucle vide |
| 17 | `window.Close();` | *(absent)* | Gestion de la fermeture |
| 18 | `}` | *(absent)* | Fermeture du `if` |
| 19 | `}` | *(absent)* | Fermeture du `while` événements |
| 20 | `// mettre à jour la logique, puis dessiner (NKCanvas)` | *(absent)* | Commentaire de suivi |
| 21 | `}` | *(absent)* | Fermeture du `while` principal |
| 22 | `return 0;` | `return 0;` | Identique |
| 23 | `}` | `}` | Identique |




