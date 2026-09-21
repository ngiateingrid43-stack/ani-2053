# Démo 2 — Le conflit résolu en direct

## Provocation du conflit devant la classe

Deux branches modifient la même ligne de `config.json` :

```bash
git checkout -b branche-x
sed -i 's/"debug": false/"debug": true/' config.json
git commit -am "active le mode debug"

git checkout main
sed -i 's/"debug": false/"debug": "verbose"/' config.json
git commit -am "passe debug en mode verbose"

git merge branche-x
```

Sortie :

```
Auto-merging config.json
CONFLICT (content): Merge conflict in config.json
Automatic merge failed; fix conflicts and then commit the result.
```

## Résolution sans couper, étape par étape

### 1. Lecture des marqueurs

```bash
cat config.json
```

```
{
<<<<<<< HEAD
  "debug": "verbose",
=======
  "debug": true,
>>>>>>> branche-x
  "port": 8080
}
```

On nomme à voix haute ce que chaque bloc représente : entre `<<<<<<<
HEAD` et `=======`, la version de la branche courante (`main`) ; entre
`=======` et `>>>>>>> branche-x`, la version qui arrive.

### 2. Décision

On explique le choix à la classe : on garde la sémantique la plus riche
(`"verbose"`) mais on veut aussi conserver la possibilité d'activer un
simple booléen ailleurs dans le code — décision : on garde `"verbose"`
comme valeur, car il inclut `true` comme cas particulier côté
consommateur du fichier.

### 3. Reconstruction

```bash
cat > config.json << 'EOF'
{
  "debug": "verbose",
  "port": 8080
}
EOF
```

Les marqueurs sont retirés à la main ; le fichier ne contient plus que le
résultat voulu.

### 4. Validation

```bash
git diff                 # plus aucun marqueur restant
git add config.json
git commit -m "Merge branch-x : conserve 'debug: verbose'"
git status                # working tree clean
```

## Ce qu'on évalue : le calme

La démonstration n'a pas cherché à éviter le conflit ni à le résoudre en
vitesse par réflexe (`git checkout --theirs` sans réfléchir) : chaque
étape a été nommée avant d'être exécutée — lecture, décision explicite et
justifiée, reconstruction propre, puis vérification qu'aucun marqueur
n'était resté avant de valider. Un conflit n'est pas une panne : c'est
Git qui demande une décision qu'il ne peut pas prendre seul.
