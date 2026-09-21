# Exo 5 — La branche, mesurée

## Mesure avant la branche

```bash
du -sh .git
```

Sortie : `188K   .git`

## Création de la branche et des trois commits

```bash
git branch experimentation
git checkout experimentation

echo "essai 1" >> notes.txt
git commit -am "essai 1"

echo "essai 2" >> notes.txt
git commit -am "essai 2"

echo "essai 3" >> notes.txt
git commit -am "essai 3"
```

## Mesure après la branche

```bash
du -sh .git
```

Sortie : `192K   .git`

## Explication du résultat

Une branche elle-même ne coûte presque rien : c'est un simple fichier de
quelques octets dans `.git/refs/heads/` contenant le hash d'un commit — la
créer (`git branch experimentation`) n'a quasiment pas fait grossir `.git`.

Ce qui a fait grossir le dépôt (+4 Ko), ce sont les **trois nouveaux
commits**, chacun avec son propre objet commit et un nouvel objet blob
pour le contenu modifié de `notes.txt`. Git ne duplique pas les fichiers
inchangés d'une branche à l'autre : les branches partagent tous les objets
qu'elles ont en commun (ici, tout l'historique avant la divergence), donc
le coût réel est proportionnel au nombre de commits nouveaux, pas au
nombre de branches.
