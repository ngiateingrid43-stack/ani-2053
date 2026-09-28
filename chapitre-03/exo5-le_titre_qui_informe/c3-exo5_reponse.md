# Exercice 5 — Le titre qui informe

## Format du titre

`"<nom du document><* si modifié> - <largeur>x<hauteur>"`, par exemple
`sansnom.txt* - 1280x720`.

## Le "bon moment" pour mettre à jour

Le titre n'est réécrit que dans trois cas précis :

1. une fois au démarrage (état initial) ;
2. quand `NkWindowResizeEvent` arrive (la taille a réellement changé) ;
3. quand F2 bascule l'état "modifié".

Il n'est **jamais** réécrit à l'intérieur de la boucle de rendu sans
condition. Deux raisons :

- `SetTitle` traverse potentiellement une API native (changement de
  bordure système) — l'appeler 60 fois par seconde est un gaspillage
  inutile ;
- sur certains systèmes, réécrire le titre en boucle peut provoquer un
  scintillement visible de la barre de titre.

## Observation 

Lors que on redimensionne la fentre le tire affiche en temps reel la taille actuel de la fenetre
et si on appuis sur F2 on stimule la modification du titre et on a l'apparition de ``*`` dans le titre