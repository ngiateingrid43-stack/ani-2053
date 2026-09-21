# Démo 1 — Le graphe au tableau

## Dépôt réel choisi

Le dépôt du moteur, autour d'une petite fenêtre de son historique
(branches `main` et `feature/inventaire`).

## Le dessin au tableau (description)

```
main:      A ── B ──────────── E ── F
                 \              /
feature:          C ──── D ────
```

- `A`, `B` : commits sur `main` avant la création de la branche.
- Point de divergence : juste après `B`, où `feature/inventaire` est
  créée.
- `C`, `D` : deux commits propres à la branche `feature/inventaire`.
- `E` : un commit indépendant arrivé sur `main` pendant que la
  fonctionnalité était en cours (travail parallèle).
- `F` : le commit de fusion — deux parents (`E` et `D`) — qui réunit les
  deux lignes.

## Le même graphe avec `git log --graph`

```bash
git log --oneline --graph --all
```

Sortie obtenue :

```
*   f6e5d4c (HEAD -> main) Merge branch 'feature/inventaire'
|\
| * d4c3b2a (feature/inventaire) ajoute le tri de l'inventaire
| * c3b2a1f ajoute la structure Item
* | e5d4c3b corrige l'affichage du HUD
|/
* b2a1f0e A jour avant la branche
* a1f0e9d commit initial
```

## Correspondance entre les deux

| Tableau | `git log --graph` |
|---|---|
| `A`, `B` | `a1f0e9d`, `b2a1f0e` |
| Point de divergence après `B` | le `*` où la ligne `|` se sépare en `|\` |
| `C`, `D` | `c3b2a1f`, `d4c3b2a` (colonne de gauche, marquée `feature/inventaire`) |
| `E` | `e5d4c3b` (colonne de droite, sur `main`) |
| `F` (fusion) | `f6e5d4c`, le commit `Merge branch...` avec deux branches `|` qui se rejoignent en `|/` |

Le dessin au tableau et la sortie de `git log --graph` racontent
exactement la même histoire : `--graph` utilise des colonnes ASCII pour
représenter ce qu'on dessine à la main — chaque `|` supplémentaire est une
ligne de développement, chaque `*` un commit, et les caractères `\` / `/`
marquent une divergence ou une fusion.
