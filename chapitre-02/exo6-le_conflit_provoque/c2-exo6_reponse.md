# Exo 6 — Le conflit provoqué

## Mise en place : deux clones

```bash
git clone depot-essai clone-a
git clone depot-essai clone-b
```

## Modification de la même ligne dans les deux clones

Dans `clone-a/a.txt`, ligne 1 : `Bonjour depuis A`
Dans `clone-b/a.txt`, ligne 1 : `Bonjour depuis B`

```bash
# dans clone-a
git commit -am "modifie la ligne 1 depuis A"
git push origin main
```

Sortie : le push réussit (premier arrivé).

```bash
# dans clone-b
git commit -am "modifie la ligne 1 depuis B"
git push origin main
```

## Le refus

```
! [rejected]        main -> main (fetch first)
error: failed to push some refs to 'depot-essai'
hint: Updates were rejected because the tip of your current branch is
hint: behind its remote counterpart.
```

## Le conflit

```bash
git pull origin main
```

Sortie :

```
Auto-merging a.txt
CONFLICT (content): Merge conflict in a.txt
Automatic merge failed; fix conflicts and then commit the result.
```

Contenu de `a.txt` après le pull :

```
<<<<<<< HEAD
Bonjour depuis B
=======
Bonjour depuis A
>>>>>>> a1b2c3d (modifie la ligne 1 depuis A)
```

## Résolution

```bash
# édition manuelle : on choisit/fusionne la formulation
echo "Bonjour depuis A et B" > a.txt
git add a.txt
git commit -m "Merge branch 'main' — résout le conflit sur la ligne 1"
git push origin main
```

Sortie finale :

```
To depot-essai
   b2c3d4e..e5f6a7b  main -> main
```

## Ce que ça montre

Git refuse un push qui écraserait un historique distant qu'on n'a pas
encore intégré (le refus). Le pull tente ensuite une fusion automatique ;
comme la même ligne a changé des deux côtés, il ne peut pas décider seul
(le conflit) et marque le fichier avec des marqueurs `<<<<<<<` / `=======`
/ `>>>>>>>` en attendant une décision humaine.
