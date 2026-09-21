# Exo 10 — Fusionner ou rejouer

## Mise en place commune

```bash
git checkout -b feature-a main
echo "a1" >> f.txt && git commit -am "a1"
echo "a2" >> f.txt && git commit -am "a2"
```

Pendant ce temps, `main` a aussi avancé :

```bash
git checkout main
echo "m1" >> g.txt && git commit -am "m1"
```

## Intégration n°1 : par fusion (`merge`)

```bash
git checkout main
git merge feature-a -m "Merge feature-a dans main"
git log --oneline --graph --all
```

Graphe obtenu :

```
*   f7e6d5c Merge feature-a dans main
|\
| * b2c3d4e a2
| * a1b2c3d a1
* | c3d4e5f m1
|/
* 1a2b3c4 commit initial
```

## Intégration n°2 : en rejouant (`rebase`)

On repart d'un état identique, mais cette fois :

```bash
git checkout feature-b     # copie de feature-a, même contenu de départ
git rebase main
git checkout main
git merge feature-b        # fast-forward, pas de commit de fusion
git log --oneline --graph --all
```

Graphe obtenu :

```
* e4d5c6b a2
* d3c4b5a a1
* c3d4e5f m1
* 1a2b3c4 commit initial
```

## Comparaison des deux graphes

- **Fusion** : conserve la forme réelle du travail — on voit qu'il y a eu
  deux lignes de développement parallèles, et un point de jonction
  explicite (le commit de merge). L'historique est fidèle aux faits mais
  plus touffu à mesure que les branches se multiplient.
- **Rebase** : réécrit les commits de `feature-a` comme s'ils avaient été
  écrits après `m1`, sur une seule ligne droite. L'historique est linéaire
  et facile à lire, mais il ne reflète plus l'ordre chronologique réel des
  événements — les hash des commits rejoués changent aussi.

## Préférence, avec argument

Je préfère lire le graphe **rejoué (rebase)** pour une branche de
fonctionnalité courte et personnelle : `git log --oneline` reste une
droite, sans bruit visuel, ce qui facilite la relecture linéaire du
travail. En revanche, je garde la **fusion** pour l'intégration d'une
branche partagée entre plusieurs personnes, car le commit de merge sert
de repère honnête — il documente qu'un ensemble cohérent de commits est
entré dans `main` à un instant donné, ce qu'un rebase effacerait.
