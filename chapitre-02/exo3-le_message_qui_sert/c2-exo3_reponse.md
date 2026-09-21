# Exo 3 — Le message qui sert

## Trois commits examinés (dépôt du moteur)

### Commit 1 — `fix: correction du calcul de collision AABB`

- **Dit-il ce qu'il fait ?** Oui, précisément : corrige le calcul de
  collision AABB.
- **Dit-il pourquoi ?** Non, aucune explication du bug corrigé.
- **Un seul sujet ?** Oui, une seule zone du code (collisions).
- **Verdict** : correct mais incomplet — un lecteur ne sait pas quel était
  le symptôme.

### Commit 2 — `update`

- **Dit-il ce qu'il fait ?** Non.
- **Dit-il pourquoi ?** Non.
- **Un seul sujet ?** Impossible à savoir sans ouvrir le diff.
- **Verdict** : message inutile — c'est le plus faible des trois.

### Commit 3 — `feat(render): ajoute le culling par frustum + corrige un fuite mémoire dans le loader de textures`

- **Dit-il ce qu'il fait ?** Oui, mais deux choses différentes.
- **Dit-il pourquoi ?** Partiellement (le "quoi" est là, pas le "pourquoi").
- **Un seul sujet ?** Non — deux sujets sans rapport direct (feature de
  rendu / correctif mémoire) mélangés dans un seul commit.
- **Verdict** : aurait dû être scindé en deux commits.

## Réécriture du message le plus faible (`update`)

Message original :

```
update
```

Message réécrit :

```
fix(physics): empêche la pénétration des objets à haute vélocité

Le solveur de collisions ne testait qu'une fois par frame, ce qui
laissait passer les objets rapides à travers les murs fins. Ajoute
un sous-pas de simulation quand la vitesse dépasse un seuil.
```

## Ce que ça enseigne

Un bon message répond à trois questions : quoi, pourquoi, et — si
nécessaire — comment. Un commit ne devrait porter qu'un seul sujet ; s'il
en faut deux mots pour le résumer avec "et", c'est probablement deux
commits.
