# Exercice 4 — Le facteur d'échelle — réponse

## Ce que le programme affiche

Le titre de la fenêtre (et le terminal, même contenu) montrent en permanence trois
valeurs, côte à côte :

- **la taille de la fenêtre** — `window.GetSize()` : la zone **client**, en pixels
  **logiques**, hors bordure et barre de titre ;
- **la taille de la cible de rendu** — `window.GetSurfaceDesc().width/height` : la
  description de la surface graphique que NKCanvas consommerait pour créer sa
  swapchain. Le champ est explicitement documenté comme étant en pixels
  **physiques** (`NkSurfaceDesc.h` : « Dimensions physiques (pixels) ») ;
- **le facteur d'échelle** — `window.GetDpiScale()` (1.0 = 100 %, 1.5 = 150 %, etc.).

Le titre/terminal ne sont réécrits que quand une de ces trois valeurs change
réellement (comparaison avec la valeur précédente), jamais à chaque frame.

## Pourquoi ces deux tailles peuvent différer

C'est exactement l'écart que documente « LE CONTRAT » dans `NkWindowConfig.h` :
`GetSize()` est toujours exprimé en coordonnées **CLIENT** (logiques), alors que
`GetSurfaceDesc()` décrit la surface en pixels **PHYSIQUES**. Sur un écran à facteur
d'échelle 1.0, les deux tailles coïncident. Sur un écran HiDPI (150 %, 200 %…), la
cible de rendu physique est plus grande que la fenêtre logique — c'est cet écart, en
pixels, que NKCanvas doit connaître pour dimensionner correctement sa swapchain sans
flou ni décalage.

## Chez moi

- Facteur d'échelle mesuré : 1.00 si écran standard
- Rapport `cible de rendu / fenêtre` observé : `1
- Resultat en sortie 
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  window.exe
     C:\Users\DELL\Desktop\FirstWindow\FirstWindow\Build\Bin\Debug-Windows\window\window.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

fenetre=960x540  cible de rendu=960x540  echelle=x1.00
fenetre=819x540  cible de rendu=819x540  echelle=x1.00
fenetre=585x540  cible de rendu=585x540  echelle=x1.00
fenetre=585x319  cible de rendu=585x319  echelle=x1.00
fenetre=585x51  cible de rendu=585x51  echelle=x1.00
fenetre=144x51  cible de rendu=144x51  echelle=x1.00
fenetre=0x0  cible de rendu=0x0  echelle=x1.00

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (348.92s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> 
```