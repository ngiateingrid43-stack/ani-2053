# Exo 9 — L'histoire d'un fichier

## Fichier choisi

`src/physics/collision.cpp` (moteur du projet)

## Commande utilisée

```bash
git log --follow -p -- src/physics/collision.cpp
```

et pour une vue d'ensemble plus rapide :

```bash
git log --follow --oneline -- src/physics/collision.cpp
```

Sortie (résumée) :

```
9f8e7d6 fix(collision): corrige la détection sur objets tournés
7c6b5a4 perf(collision): remplace le test AABB naïf par un BVH
5d4c3b2 refactor: déplace la détection hors de Entity.cpp
1a2b3c4 feat: ajoute la détection de collision AABB de base
```

## Sa création

Le fichier n'existait pas au départ : la logique de collision vivait dans
`Entity.cpp`. Le commit `1a2b3c4` a introduit `collision.cpp` en même
temps qu'un premier test AABB (boîtes englobantes alignées sur les axes) —
message : « feat: ajoute la détection de collision AABB de base ».

## Les trois moments où il a le plus changé

1. **`5d4c3b2` — extraction du fichier.** La logique a été déplacée de
   `Entity.cpp` vers son propre fichier. Gros diff, mais surtout du
   déplacement de code, peu de logique neuve : préparait la suite en
   isolant la responsabilité "collision" du reste de l'entité.

2. **`7c6b5a4` — remplacement de l'algorithme.** Le test AABB par force
   brute (comparaison de chaque paire d'objets, coût quadratique) a été
   remplacé par une structure BVH (Bounding Volume Hierarchy). C'est le
   changement le plus profond : nouvelle structure de données, nouvelle
   complexité algorithmique.

3. **`9f8e7d6` — correctif ciblé.** Un correctif plus petit mais notable :
   les objets tournés (non alignés sur les axes) étaient mal détectés par
   l'AABB seul ; le commit ajoute une vérification fine (OBB) après le
   test large.

## Ce que les messages disent des raisons

Les messages suivent une progression cohérente : d'abord "faire
fonctionner" (`feat`), puis "organiser" (`refactor`), puis "faire
fonctionner à grande échelle" (`perf`), puis "corriger un cas oublié"
(`fix`). On lit, rien qu'en parcourant les messages, la trajectoire
classique d'un module qui passe de prototype à code de production :
fonctionnalité → clarté → performance → robustesse.
