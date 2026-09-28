# Exercice 12 — L'inventaire des écrans

## Ce que fait le programme

`window.EnumerateMonitors()` renvoie, à chaque appel, la liste à jour des
écrans branchés (taille, position dans l'espace virtuel multi-écrans,
échelle DPI, rafraîchissement, et lequel est primaire). On affiche cet
inventaire au démarrage, puis à chaque fois que la position de la
fenêtre change — signe possible d'un passage d'un écran à l'autre.
`window.GetCurrentMonitor()` indique l'écran qui contient effectivement
la fenêtre.

## Piège évité

Le premier élément de `EnumerateMonitors()` n'est pas garanti être
l'écran principal : on ne s'y fie jamais, on teste `isPrimary`.

## Résultat observé 

```
--- 1 ecran(s) detecte(s) ---
Ecran 0 : 1600x900 @ 60Hz, echelle x1.00, pos(0,0), primaire=oui, nom=\\.\DISPLAY1
>>> La fenetre est actuellement sur l'ecran 0 (\\.\DISPLAY1)

--- 1 ecran(s) detecte(s) ---
Ecran 0 : 1600x900 @ 60Hz, echelle x1.00, pos(0,0), primaire=oui, nom=\\.\DISPLAY1
>>> La fenetre est actuellement sur l'ecran 0 (\\.\DISPLAY1)

--- 1 ecran(s) detecte(s) ---
Ecran 0 : 1600x900 @ 60Hz, echelle x1.00, pos(0,0), primaire=oui, nom=\\.\DISPLAY1
>>> La fenetre est actuellement sur l'ecran 0 (\\.\DISPLAY1)

--- 1 ecran(s) detecte(s) ---
Ecran 0 : 1600x900 @ 60Hz, echelle x1.00, pos(0,0), primaire=oui, nom=\\.\DISPLAY1
>>> La fenetre est actuellement sur l'ecran 0 (\\.\DISPLAY1)

--- 1 ecran(s) detecte(s) ---
Ecran 0 : 1600x900 @ 60Hz, echelle x1.00, pos(0,0), primaire=oui, nom=\\.\DISPLAY1
>>> La fenetre est actuellement sur l'ecran 0 (\\.\DISPLAY1)

--- 1 ecran(s) detecte(s) ---
Ecran 0 : 1600x900 @ 60Hz, echelle x1.00, pos(0,0), primaire=oui, nom=\\.\DISPLAY1
>>> La fenetre est actuellement sur l'ecran 0 (\\.\DISPLAY1)
```

