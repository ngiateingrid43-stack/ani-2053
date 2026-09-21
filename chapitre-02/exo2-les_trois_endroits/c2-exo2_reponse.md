# Exo 2 — Les trois endroits

## Étape 1 : modification du fichier (avant add)

```bash
echo "Ligne ajoutée" >> a.txt
git status
```

Sortie :

```
On branch main
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   a.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

## Étape 2 : après `git add`

```bash
git add a.txt
git status
```

Sortie :

```
On branch main
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        modified:   a.txt
```

## Étape 3 : après `git commit`

```bash
git commit -m "Complète a.txt"
git status
```

Sortie :

```
On branch main
nothing to commit, working tree clean
```

## Ce qui change entre les trois

- **Avant `add`** : le fichier n'existe que dans le répertoire de travail ;
  git voit une différence mais ne l'a pas encore capturée.
- **Après `add`** : la modification est passée dans la zone d'index (le
  "stage") — un instantané prêt à être figé, distinct du répertoire de
  travail.
- **Après `commit`** : l'index est vidé dans l'objet commit, qui rejoint
  l'historique du dépôt (`.git`). Les trois endroits — répertoire de
  travail, index, dépôt — sont de nouveau synchronisés, d'où le message
  "nothing to commit, working tree clean".
