# Exo 11 — Le fichier qu'on n'aurait pas dû

## Mesure avant

```bash
du -sh .git
```

Sortie : `196K   .git`

## Commit volontaire d'un fichier de 10 Mo

```bash
dd if=/dev/urandom of=gros_fichier.bin bs=1M count=10
git add gros_fichier.bin
git commit -m "ajoute (par erreur) un gros fichier binaire"
```

```bash
du -sh .git
```

Sortie : `9.9M   .git`

## Retrait au commit suivant

```bash
git rm gros_fichier.bin
git commit -m "retire gros_fichier.bin"
```

```bash
du -sh .git
```

Sortie : `9.9M   .git` — **inchangée**.

## Mesure finale après nettoyage forcé (pour comparaison)

```bash
git gc --prune=now
du -sh .git
```

Même après un garbage collection classique, la taille ne redescend pas :
`9.9M .git`.

## Conclusion

Supprimer un fichier dans un nouveau commit ne fait que dire à Git de ne
plus le suivre **à partir de maintenant** — cela ajoute un commit qui
retire le fichier du contenu courant, mais l'objet blob de 10 Mo reste
stocké dans `.git`, atteignable depuis l'historique (le commit précédent
le référence toujours). `git rm` n'est donc pas un effacement : c'est un
nouveau commit qui masque le fichier en avant, sans toucher au passé.

Pour réellement faire disparaître les octets, il faut réécrire
l'historique — par exemple avec `git filter-repo` ou `BFG Repo-Cleaner` —
puis exécuter `git gc` pour purger les objets devenus inaccessibles. Sans
cette étape, le dépôt continue de porter le poids d'un fichier qu'on croit
avoir retiré.
