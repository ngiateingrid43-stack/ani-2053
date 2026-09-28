# Exercice 7 — Le glisser qui sort

## Sans capture (`CaptureMouse(false)`, valeur par défaut)

Dès que le curseur franchit le bord de la zone client pendant le
glisser, la fenêtre cesse de recevoir des `NkMouseMoveEvent` — comme
n'importe quelle application desktop classique : le suivi de la souris
appartient à la fenêtre qui se trouve *sous* le curseur à cet instant, ou
à aucune si le curseur est au-dessus du bureau. Le glisser "se perd"
visuellement dès la sortie.

## Avec capture (`CaptureMouse(true)`)

Le système continue de router les événements souris vers la fenêtre même
quand le curseur est sorti de sa zone client (voire de l'écran, selon la
plateforme). C'est ce que font les éditeurs de code pour une sélection
de texte qui continue au-delà du bord visible, ou les jeux pour un drag
de caméra sans limite. Le journal (`std::printf`) continue donc de
recevoir des positions même hors zone client.

## Différence côté utilisateur 

Lors que on ouvre plusieurs fenetre dans le cadre de nos activite personnelle le fait que une de ces application a une capture active cela pourra continuer a effectuter des operations involontaire du a la prise continue de la position de la souris sur l'ecran 
