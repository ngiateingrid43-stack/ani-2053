# Exo 7 — Le conflit qui n'en est pas un

## Mise en place

Fichier `article.txt` de départ (10 lignes), partagé par deux clones,
`clone-a` et `clone-b`.

Dans `clone-a`, modification de la **ligne 2** :

```bash
sed -i '2s/.*/Introduction révisée par A/' article.txt
git commit -am "révise l'introduction"
git push origin main
```

Dans `clone-b`, modification de la **ligne 9** (loin de la ligne 2) :

```bash
sed -i '9s/.*/Conclusion révisée par B/' article.txt
git commit -am "révise la conclusion"
```

## Récupération et fusion automatique

```bash
git pull origin main
```

Sortie :

```
Auto-merging article.txt
Merge made by the 'ort' strategy.
 article.txt | 2 +-
 1 file changed, 2 insertions(+), 2 deletions(-)
```

Aucun marqueur de conflit n'apparaît. Le fichier final contient bien les
deux modifications : la ligne 2 vient de A, la ligne 9 vient de B.

```bash
git push origin main
```

## Ce que ça montre

Git ne compare pas les fichiers dans leur ensemble : il compare des
régions de lignes modifiées (des "hunks") par rapport à un ancêtre commun.
Quand deux modifications touchent des zones suffisamment éloignées d'un
même fichier, les deux ensembles de lignes changées ne se recouvrent pas,
et l'algorithme de fusion à trois voies (base commune + version A +
version B) peut appliquer les deux sans ambiguïté. Le conflit n'apparaît
que lorsque les mêmes lignes — ou des lignes adjacentes — sont touchées
des deux côtés.
