# Démo 4 — La revue de branche

## Branche reçue : `feature/classement-scores`

```bash
git log --oneline main..feature/classement-scores
```

```
f1e2d3c ajoute l'affichage du classement
e0d1c2b tri les scores
d9c0b1a wip
c8b9a0f ajoute la table des scores
```

## Ce qu'elle fait

Elle introduit un classement des meilleurs scores : une nouvelle structure
de données pour stocker les scores, un tri, et un écran d'affichage. Le
comportement global est clair et testable manuellement.

## Les commits sont-ils lisibles ?

- `c8b9a0f`, `e0d1c2b`, `f1e2d3c` : messages clairs, un sujet chacun.
- `d9c0b1a "wip"` : illisible en l'état — ne dit rien du contenu. À
  fusionner (squash) dans le commit voisin avant intégration, ou à
  réécrire avec un vrai message.

## Ce qui manque

- Aucun test automatisé pour le tri des scores (cas limite : scores
  ex æquo — ordre non spécifié).
- Pas de gestion du cas où la liste de scores est vide (l'écran
  d'affichage suppose au moins un élément).
- Le message du commit `f1e2d3c` ne précise pas si l'affichage gère plus
  de 10 entrées (pagination ?).

## Ce qui ne devrait pas y être

- Le commit `d9c0b1a` contient une ligne `print("debug ici")` oubliée
  dans le code — à retirer avant fusion.
- Un fichier `scores_test.json` de test personnel a été committé par
  erreur dans `c8b9a0f` ; il n'a rien à faire dans le dépôt partagé.

## Revue écrite (verdict)

**Décision : changements demandés, pas encore prête à fusionner.**

1. Squasher ou réécrire le commit `wip`.
2. Retirer l'instruction de debug oubliée.
3. Retirer `scores_test.json` du dépôt (ou le déplacer dans un dossier de
   fixtures ignoré).
4. Ajouter au moins un test sur le cas "liste de scores vide" et un sur
   les ex æquo.

## Discussion à quatre (notes)

Le groupe s'accorde sur les quatre points ci-dessus. Débat sur l'ordre des
ex æquo : décision prise à l'unanimité de conserver l'ordre d'insertion
(stable sort), documentée dans le prochain commit. Ré-évaluation prévue
une fois les points corrigés, avant fusion dans `main`.
