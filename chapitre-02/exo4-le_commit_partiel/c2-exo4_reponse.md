# Exo 4 — Le commit partiel

## Les deux modifications sans rapport

Dans `config.py` :
1. Correction d'une faute de frappe dans un commentaire (`# initalise` →
   `# initialise`).
2. Changement de la valeur par défaut du port du serveur (`8000` → `8080`).

## Séparer avec `git add -p`

```bash
git add -p config.py
```

Sortie (extrait pour le premier hunk — le commentaire) :

```
@@ -3,7 +3,7 @@
-# initalise la config
+# initialise la config
Stage this hunk [y,n,q,a,d,s,e,?]? y
```

```bash
git commit -m "docs: corrige une faute dans un commentaire"
```

Puis pour le second hunk (le port) :

```bash
git add -p config.py
```

```
@@ -10,7 +10,7 @@
-PORT = 8000
+PORT = 8080
Stage this hunk [y,n,q,a,d,s,e,?]? y
```

```bash
git commit -m "config: change le port par défaut à 8080"
```

## Vérification dans l'historique

```bash
git log -p -2
```

Le premier commit ne montre que la ligne de commentaire modifiée ; le
second ne montre que la ligne `PORT`. Chaque commit ne contient bien que
son propre sujet — aucun mélange des deux changements.

## Ce que ça montre

`git add -p` découpe un fichier modifié en "hunks" (blocs de changement
contigus) et permet de les mettre en scène un par un. Cela rend possible
des commits atomiques même quand deux modifications indépendantes se
trouvent, par accident, dans le même fichier.
