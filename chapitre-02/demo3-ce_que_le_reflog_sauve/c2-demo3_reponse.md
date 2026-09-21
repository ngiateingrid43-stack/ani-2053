# Démo 3 — Ce que le `reflog` sauve

## État avant destruction

```bash
git log --oneline
```

```
d4e5f6a (HEAD -> main) ajoute la sauvegarde automatique
c3d4e5f corrige le calcul du score
b2c3d4e ajoute l'écran de pause
a1b2c3d commit initial
```

## Destruction volontaire

```bash
git reset --hard HEAD~2
```

Sortie :

```
HEAD is now at b2c3d4e ajoute l'écran de pause
```

## Constat de la perte

```bash
git log --oneline
```

```
b2c3d4e (HEAD -> main) ajoute l'écran de pause
a1b2c3d commit initial
```

Les commits `c3d4e5f` ("corrige le calcul du score") et `d4e5f6a` ("ajoute
la sauvegarde automatique") ont disparu de `git log` : ils ne sont
référencés par aucune branche. Un simple `git log` ne les montre plus —
pour la classe, l'impression est celle d'un travail réellement perdu.

## Retrouvé par le reflog

```bash
git reflog
```

Sortie :

```
b2c3d4e HEAD@{0}: reset: moving to HEAD~2
d4e5f6a HEAD@{1}: commit: ajoute la sauvegarde automatique
c3d4e5f HEAD@{2}: commit: corrige le calcul du score
b2c3d4e HEAD@{3}: commit: ajoute l'écran de pause
a1b2c3d HEAD@{4}: commit (initial): commit initial
```

Le reflog enregistre chaque position occupée par `HEAD` sur cette machine,
y compris `d4e5f6a`, pourtant absent du graphe de commits.

```bash
git reset --hard d4e5f6a
git log --oneline
```

```
d4e5f6a (HEAD -> main) ajoute la sauvegarde automatique
c3d4e5f corrige le calcul du score
b2c3d4e ajoute l'écran de pause
a1b2c3d commit initial
```

Les deux commits sont restaurés à l'identique — mêmes hash, même contenu.

## Ce que la démonstration enseigne

Un `reset --hard` ne détruit pas immédiatement les objets commit : il
déplace seulement la référence de branche et la pointe `HEAD`. Les
commits orphelins restent physiquement présents dans `.git/objects`
jusqu'à un `git gc` (généralement après plusieurs semaines pour les objets
inatteignables). Le `reflog`, propre à chaque dépôt local et non partagé
par un push, garde la trace de tous ces déplacements et permet de revenir
en arrière tant que le nettoyage automatique n'a pas eu lieu.
