# c4-exo3 — Un seul chemin de fermeture

Troisième exercice du chapitre 4 (fenêtrage et événements) avec le framework **Nkentseu** (`NKWindow`).

## Énoncé

Faire en sorte que la fenêtre se ferme sur l'événement de fermeture, et seulement sur lui. Vérifier que le bouton du système (la croix), le raccourci du gestionnaire de fenêtres (Alt+F4) et votre propre touche passent tous les trois par le **même chemin**.

## Objectifs pédagogiques

- Comprendre que la fermeture d'une fenêtre est d'abord une **demande** (`NkWindowCloseEvent`), pas une action immédiate.
- Centraliser la logique de fermeture dans **une seule fonction**, pour pouvoir y ajouter plus tard une confirmation, une sauvegarde ou un nettoyage.
- Vérifier ce chemin unique par un compteur.

## Fonctionnement du programme

1. Création d'une fenêtre 700×300 centrée.
2. Une variable globale `gNbFermetures` compte les passages dans la fonction `TraiterFermeture()`.
3. `TraiterFermeture(window, origine)` :
   - incrémente le compteur ;
   - affiche l'origine de la demande et le numéro d'appel ;
   - appelle `window.Close()`.
4. La boucle d'événements appelle cette fonction dans deux cas :
   - `NkWindowCloseEvent` : croix, Alt+F4, gestionnaire de fenêtres ;
   - `NkKeyPressEvent` avec `NkKey::NK_Q` : la touche personnelle.
5. À la sortie de la boucle, le programme affiche le nombre total d'appels au chemin de fermeture.

## Résultats

| Essai | Origine affichée | Appels | Durée | Fin |
|---|---|---|---|---|
| 1 | ma touche Q | 1 | 8,26 s | normale |
| 2 | événement de fermeture (croix / Alt+F4 / gestionnaire) | 1 | 5,83 s | normale |
| 3 | événement de fermeture (croix / Alt+F4 / gestionnaire) | 1 | 5,69 s | normale |

Message final identique dans les trois cas : `boucle terminee proprement (1 appel(s) au chemin de fermeture)`.

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  window.exe
     C:\Users\DELL\Desktop\FirstWindow\FirstWindow\Build\Bin\Debug-Windows\window\window.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

fermeture demandee (origine : ma touche Q) -> chemin unique, appel n.1
boucle terminee proprement (1 appel(s) au chemin de fermeture)

  ◀  FIN D'EXECUTION  —  termine normalement  (8.26s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> jenga r

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝


━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  window.exe
     C:\Users\DELL\Desktop\FirstWindow\FirstWindow\Build\Bin\Debug-Windows\window\window.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

fermeture demandee (origine : evenement de fermeture (croix / Alt+F4 / gestionnaire)) -> chemin unique, appel n.1
boucle terminee proprement (1 appel(s) au chemin de fermeture)

  ◀  FIN D'EXECUTION  —  termine normalement  (5.83s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> jenga r

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝


━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  window.exe
     C:\Users\DELL\Desktop\FirstWindow\FirstWindow\Build\Bin\Debug-Windows\window\window.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

fermeture demandee (origine : evenement de fermeture (croix / Alt+F4 / gestionnaire)) -> chemin unique, appel n.1
boucle terminee proprement (1 appel(s) au chemin de fermeture)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (5.69s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> 
```

## Analyse

- **Le chemin est bien unique** : quelle que soit l'origine, le compteur vaut exactement 1 et l'exécution se termine normalement.
- **Croix et Alt+F4 sont indistinguables** : Windows les transforme tous deux en demande de fermeture, et Nkentseu les livre sous la forme du même `NkWindowCloseEvent`. C'est ce que confirme le message identique dans les essais 2 et 3 (probablement la croix puis Alt+F4 ; le journal ne les distingue pas, c'est justement le but).
- **La touche personnelle rejoint le même chemin** grâce à la fonction commune `TraiterFermeture()`.

## Point d'attention

La touche Q appelle `TraiterFermeture()` **directement**, sans passer par un `NkWindowCloseEvent`. Le chemin de *code* est bien commun, mais le chemin d'*événement* diffère : si la consigne « seulement sur l'événement de fermeture » est lue strictement, la touche Q devrait plutôt **demander** la fermeture (par exemple en injectant ou en déclenchant l'événement de fermeture, si l'API de Nkentseu le permet) et laisser la boucle traiter cet événement comme les autres.

Autre cas limite non testé : si une touche Q et un événement de fermeture arrivent dans la même image, `TraiterFermeture()` serait appelée deux fois (compteur à 2). Ici, cela reste sans danger, car `window.Close()` peut être appelée plusieurs fois, mais une protection (un indicateur « fermeture déjà demandée ») rendrait le chemin réellement idempotent.

