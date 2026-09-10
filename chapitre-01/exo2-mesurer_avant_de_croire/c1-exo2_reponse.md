# Exercice2: Analyse du dépôt

## Commande powershell et resultat

**Nombre de fichiers source**
```
PS C:\Users\DELL> (Get-ChildItem -Path "C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1" -File -Recurse).Count
14644
PS C:\Users\DELL> $root = "C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1"
>>
>> # Tous les fichiers source C/C++
>> Get-ChildItem $root -Recurse -File -Include *.c,*.cpp,*.cc,*.cxx,*.h,*.hpp,*.hxx |
>>     Group-Object Extension |
>>     Sort-Object Count -Descending |
>>     Format-Table Name, Count -AutoSize
>>
>> # Total fichiers sources
>> (Get-ChildItem $root -Recurse -File -Include *.c,*.cpp,*.cc,*.cxx,*.h,*.hpp,*.hxx).Count

Name Count
---- -----
.h    1787
.cpp  1183
.c     248
.cc    221
.hpp    60


3499
```

**Nombre de ligne**
```
PS C:\Users\DELL> $root = "C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1"
>>
>> $headers = Get-ChildItem $root -Recurse -File -Include *.h,*.hpp,*.hxx
>> $sources = Get-ChildItem $root -Recurse -File -Include *.c,*.cpp,*.cc,*.cxx
>>
>> function Count-Lines($list) {
>>     ($list | ForEach-Object { (Get-Content $_.FullName | Measure-Object -Line).Lines } | Measure-Object -Sum).Sum
>> }
>>
>> "Headers : {0} fichiers, {1} lignes" -f $headers.Count, (Count-Lines $headers)
>> "Sources : {0} fichiers, {1} lignes" -f $sources.Count, (Count-Lines $sources)
Headers : 1847 fichiers, 1010645 lignes
Sources : 1652 fichiers, 813197 lignes


## Chiffres annoncés (énoncé)

| Métrique | Valeur |
|---|---|
| Fichiers source | 2 641 |
| Lignes | 1 193 385 |
| Fichiers projet | 221 |
| Taille disque | 17 Go |
```

## Comptage réel

```powershell
# Tous fichiers
(Get-ChildItem $root -Recurse -File).Count
# → 14 644

# Sources C/C++
Get-ChildItem $root -Recurse -File -Include *.c,*.cpp,*.cc,*.cxx,*.h,*.hpp,*.hxx |
  Group-Object Extension | Sort Count -Descending
# .h 1787 | .cpp 1183 | .c 248 | .cc 221 | .hpp 60 → TOTAL 3499

# Lignes
Headers (.h/.hpp/.hxx) : 1847 fichiers → 1 010 645 lignes
Sources (.c/.cpp/.cc/.cxx) : 1652 fichiers → 813 197 lignes
TOTAL : 3499 fichiers → 1 823 842 lignes
```

## Comparaison

| Métrique | Énoncé | Réel | Écart |
|---|---|---|---|
| Fichiers source | 2 641 | 3 499 | +858 |
| Lignes | 1 193 385 | 1 823 842 | +630 457 |

## Pourquoi l'écart ?

1. **En-têtes comptés ?** — Oui ici (+1 847 fichiers / +1 M lignes). Le chapitre compte probablement surtout les `.cpp/.c/.cc`.
2. **Dossier `Build/` ?** — Inclus ici → gonfle artificiellement (fichiers générés). À exclure pour coller à l'énoncé.
3. **Tests ?** — Inclus ici (`test/`, `*_test.cpp`). Le chapitre les exclut probablement.
4. **Méthode** — `Measure-Object -Line` compte **toutes** les lignes (vides + commentaires). `cloc` donnerait moins.
5. **Version du dépôt** — Le chapitre peut refléter un état antérieur.

## Conclusion

Le chiffre **2 641 fichiers / 1 193 385 lignes** correspond à un périmètre **restreint** :
- sources d'implémentation + headers publics,
- **hors** `Build/`,
- **hors** tests.

Le comptage brut donne **3 499 fichiers / 1 823 842 lignes** car il inclut tout. L'écart n'est **pas une erreur** mais une **différence de définition** du périmètre.