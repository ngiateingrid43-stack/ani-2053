# Exercice 6 — Les sept curseurs

## API utilisée [à vérifier]

Le guide NKWindow fourni ne documente pas d'API de curseur au-delà de
`ShowMouse(bool)`. Ce code suppose `window.SetCursor(NkCursorType::...)`.
**Avant de pousser**, vérifie dans `NKWindow.h` le nom réel du type et de
la méthode, et ajuste l'énumération `NkCursorType` en conséquence
(valeurs disponibles réellement supportées par le moteur).

## Découpage en 7 zones

La fenêtre est divisée en 7 bandes verticales égales ; le curseur change
de forme dès que `NkMouseMoveEvent` fait passer la souris d'une bande à
l'autre.

## Deuxième partie — un seul dépôt au démarrage

Si l'on ne pose le curseur qu'**une seule fois**, au démarrage (variante
en commentaire dans le code), alors la forme reste figée sur celle posée
initialement, quelle que soit la zone survolée par la suite : rien dans
la boucle ne rappelle `SetCursor`, donc rien ne le change. Selon la
plateforme, il est aussi possible que l'OS réinitialise lui-même le
curseur à sa forme par défaut (flèche) dès qu'il ré-entre dans la zone
client, ce qui annulerait même ce dépôt initial.

## Observation [TODO]

Résultat exact observé chez toi avec la variante "un seul dépôt" :
[TODO — à noter en testant sur ta plateforme]
