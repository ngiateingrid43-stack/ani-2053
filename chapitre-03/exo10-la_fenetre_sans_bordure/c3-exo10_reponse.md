# Exercice 10 — La fenêtre sans bordure — réponse

## Ce qui est implémenté

- `cfg.frame = false` : aucune bordure ni barre de titre système.
- Une barre de titre "maison" purement **logique** (bande de 32 px en haut) :
  - le reste de la bande (hors boutons) est la zone de **déplacement** : un clic gauche
    dedans appelle `window.BeginDragMove()`, qui délègue le déplacement à l'OS
    (hand-off natif) — on n'a pas à recalculer la position à chaque `NkMouseMoveEvent`.
  - les 46 derniers pixels de droite = **Fermer** (`window.Close()`).
  - les 46 pixels avant = **Agrandir/Restaurer**, qui bascule selon `IsMaximized()`.
  - les 46 pixels avant = **Réduire** (`window.Minimize()`).
  - un double-clic dans la zone de déplacement bascule aussi agrandir/restaurer.

## Ce qui manque, et pourquoi

Ce chapitre n'emploie que NKWindow + NKMain : il n'y a **aucun rendu** (pas de
NKCanvas), donc pas de rectangles ni de glyphes réellement dessinés à l'écran pour ces
trois boutons — seules les zones de détection (hit-test) existent, confirmées par la
console. Brancher NKCanvas (chapitre 5) permettrait de peindre par-dessus exactement
les mêmes coordonnées (`kBarHeight`, `kButtonWidth`) sans changer la logique d'entrée.



