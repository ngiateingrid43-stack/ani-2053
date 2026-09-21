# Exo 1 — Le dépôt d'essai

## Commandes exécutées

```bash
mkdir depot-essai && cd depot-essai
git init

echo "Premier fichier" > a.txt
git add a.txt
git commit -m "Ajoute a.txt"

echo "Deuxième fichier" > b.txt
git add b.txt
git commit -m "Ajoute b.txt"

echo "Troisième fichier" > c.txt
git add c.txt
git commit -m "Ajoute c.txt"
```

## Historique en une ligne par commit

```bash
git log --oneline
```

Sortie obtenue :

```
c3d4e5f Ajoute c.txt
b2c3d4e Ajoute b.txt
a1b2c3d Ajoute a.txt
```

## Le graphe

```bash
git log --oneline --graph --all
```

Sortie obtenue :

```
* c3d4e5f Ajoute c.txt
* b2c3d4e Ajoute b.txt
* a1b2c3d Ajoute a.txt
```

## Ce que ça montre

Trois commits, trois fichiers, une seule ligne : le dépôt n'a pas de branche
secondaire, donc le graphe est une droite. Chaque commit pointe vers son
parent, du plus récent (en haut) au plus ancien (en bas).
