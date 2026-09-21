# Exo 8 — Six manières de défaire

## 1. Une modification non voulue (avant `add`)

```bash
echo "erreur" >> notes.txt
git status          # modified: notes.txt
git restore notes.txt
git status           # nothing to commit, working tree clean
```

`git restore` recopie la version du fichier telle qu'elle était dans le
dernier commit, effaçant la modification non ajoutée.

## 2. Un `add` de trop

```bash
git add fichier_prive.env
git status            # Changes to be committed: fichier_prive.env
git restore --staged fichier_prive.env
git status            # redevient "not staged" / "untracked"
```

`git restore --staged` retire le fichier de l'index sans toucher au
répertoire de travail : le fichier reste modifié, mais n'est plus en file
d'attente pour le prochain commit.

## 3. Un commit de trop (pas encore poussé)

```bash
git commit -m "commit fait par erreur"
git reset --soft HEAD~1
git status             # les changements reviennent dans l'index
```

`git reset --soft` déplace la branche vers le commit précédent tout en
gardant l'index intact : le contenu du commit annulé se retrouve prêt à
être recommité (ou modifié).

## 4. Un commit poussé qu'il faut annuler

```bash
git revert HEAD
git push origin main
```

Sortie :

```
[main f1a2b3c] Revert "commit erroné"
 1 file changed, 1 insertion(+), 1 deletion(-)
```

`git revert` ne réécrit pas l'historique : il crée un **nouveau** commit
qui applique l'inverse du commit visé — sûr sur une branche déjà partagée,
contrairement à `reset`.

## 5. Un travail en cours qu'il faut mettre de côté

```bash
git stash push -m "travail en cours sur le menu"
git checkout autre-branche
# ... travail sur autre-branche ...
git checkout branche-initiale
git stash pop
```

`git stash` range les modifications non commitées (index + répertoire de
travail) dans une pile séparée, laissant l'arbre de travail propre pour
changer de branche sans perdre le travail.

## 6. Un commit « perdu » retrouvé par le reflog

```bash
git reset --hard HEAD~2   # perte volontaire de deux commits
git log --oneline          # les deux commits n'apparaissent plus

git reflog
```

Sortie du reflog :

```
a1b2c3d HEAD@{0}: reset: moving to HEAD~2
d4e5f6a HEAD@{1}: commit: ajoute la fonctionnalité X
c3d4e5f HEAD@{2}: commit: corrige le bug Y
```

```bash
git reset --hard d4e5f6a
```

Le reflog conserve la trace de chaque déplacement de HEAD, même après un
`reset --hard` ; il permet de retrouver et de restaurer un commit qui
n'est plus atteignable depuis aucune branche.

## Synthèse

| Situation | Commande | Portée |
|---|---|---|
| Modif. non voulue | `git restore <f>` | répertoire de travail |
| Add de trop | `git restore --staged <f>` | index |
| Commit de trop (local) | `git reset --soft HEAD~1` | historique local |
| Commit poussé | `git revert HEAD` | nouveau commit, historique préservé |
| Travail à mettre de côté | `git stash` | pile séparée, temporaire |
| Commit perdu | `git reflog` + `git reset --hard` | retrouve un commit orphelin |
