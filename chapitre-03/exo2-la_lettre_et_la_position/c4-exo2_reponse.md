# c4-exo2 — Position physique et lettre d'une touche

Deuxième exercice du chapitre 4 (fenêtrage et événements) avec le framework **Nkentseu** (`NKWindow`).

## Énoncé

Afficher, pour chaque touche pressée, sa **lettre** et son **code physique**. Changer la disposition du clavier dans le système, recommencer, et noter ce qui change.

Test réalisé avec un clavier **allemand (QWERTZ)** puis **français (AZERTY)**.

## Objectifs pédagogiques

- Distinguer deux informations que l'on confond souvent :
  - la **position physique** de la touche (`NkKeyPressEvent` → `GetKey()`), indépendante de la disposition ;
  - le **caractère produit** (`NkTextInputEvent` → `GetUtf8()` / `GetCodepoint()`), qui dépend de la disposition, des modificateurs et des touches mortes.
- Savoir quel événement utiliser selon le besoin (déplacer un personnage ou saisir du texte).

## Fonctionnement du programme

1. Création d'une fenêtre 700×300 centrée.
2. Boucle d'événements avec `NkEvents().PollEvent()` :
   - `NkWindowCloseEvent` : fermeture de la fenêtre ;
   - `NkKeyPressEvent` : affiche le code `NkKey` et l'état des modificateurs (Ctrl, Shift, Alt, AltGr). **Échap** quitte ;
   - `NkTextInputEvent` : si le caractère est imprimable, affiche la lettre et son point de code Unicode.
3. Pause de 16 ms par itération (~60 images/s).

## Résultats

### Clavier allemand (QWERTZ)

Touches pressées, de gauche à droite sur la rangée du haut :

| Position (`NkKey`) | 41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Lettre produite | q | w | e | r | t | **z** | u | i | o | p | **ü** | **+** |

Puis trois touches de code 112, 109 et 108 ont produit `5`, `2` et `1` (très probablement le pavé numérique).

### Clavier français (AZERTY)

| Position (`NkKey`) | 41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50 |
|---|---|---|---|---|---|---|---|---|---|---|
| Lettre produite | **a** | **z** | e | r | t | **y** | u | i | o | p |

Puis rangée du milieu :

| Position (`NkKey`) | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 |
|---|---|---|---|---|---|---|---|---|
| Lettre produite | q | s | d | f | g | h | j | k |

## Ce qui change (et ce qui ne change pas)

| | Change avec la disposition ? |
|---|---|
| Code `NkKey` (position physique) | **Non** : la touche 41 est toujours la première de la rangée du haut |
| Lettre / point de code Unicode | **Oui** : 41 donne `q` en allemand et `a` en français |
| Modificateurs (Ctrl, Shift, Alt, AltGr) | Non, ils ne dépendent pas de la disposition |

Exemples visibles dans les résultats :

- **Touche 41** : `q` (DE) et `a` (FR).
- **Touche 42** : `w` (DE) et `z` (FR).
- **Touche 46** : `z` (DE) et `y` (FR) : ce n'est pas la même lettre, mais c'est la même touche physique.
- **Touches 51 et 52** : `ü` et `+` en allemand. Elles n'ont pas été testées en français.
- **Rangée du milieu** : le code 55 correspond au `q` français.


