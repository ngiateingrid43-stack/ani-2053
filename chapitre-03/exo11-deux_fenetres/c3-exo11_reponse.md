# Exercice 11 — Deux fenêtres

## Identification de la source

Tous les événements passent par le même système global `NkEvents()`,
mais chaque `NkEvent` porte l'identifiant de sa fenêtre source via
`GetWindowId()`. On compare cet id à `fenetreA.GetId()` /
`fenetreB.GetId()` pour savoir laquelle a reçu le clic.

## Résultat

```
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> jenga run   

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

Clic recu par Fenetre A
Clic recu par Fenetre B
Clic recu par Fenetre A
Clic recu par Fenetre B
Clic recu par Fenetre A
Clic recu par Fenetre B
Clic recu par Fenetre A
Clic recu par Fenetre B
Clic recu par Fenetre A
Clic recu par Fenetre B
Clic recu par Fenetre A
Clic recu par Fenetre B
Clic recu par Fenetre A

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (135.53s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> 
```

## Ce qu'il manquerait pour dessiner dans les deux

Une seule `NkRenderWindow` est liée à **une** `NkWindow`. Pour dessiner
dans les deux fenêtres il faudrait :

1. créer **deux** cibles de rendu (`NkRenderWindow rtA(fenetreA, desc)`,
   `NkRenderWindow rtB(fenetreB, desc)`) ;
2. dans la boucle, après avoir vidé la file d'événements, faire
   `rtA.Clear(...) / ... / rtA.Display()` puis la même chose pour `rtB` ;
3. si l'on utilise NKUI, dupliquer aussi le contexte UI et son backend
   (un `NkUIContext` + `NkUICanvasBackend` par fenêtre), car le draw
   list et l'état d'entrée sont propres à une surface.

Chaque fenêtre reste donc indépendante côté rendu, seule la boucle
d'événements est partagée.
