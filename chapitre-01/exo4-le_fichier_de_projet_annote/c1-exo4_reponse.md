# Exercice 4 Annotation ligne par ligne - `NKCore.jenga`

Module analysé : **NKCore** (types, macros, assertions, opérations bit - C++20), dépendant de **NKPlatform**.

---

## 1. En-tête et imports

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
NKCore - Types, macros, assertions, opérations bit (C++20)
===========================================================
S'appuie sur NKPlatform pour les types fixes et macros fondamentaux
réutilisés par toutes les couches supérieures.
"""

from Jenga import *
from jengaconfig import *
```

| Ligne | Rôle |
|---|---|
| `#!/usr/bin/env python3` | Shebang - indique que le fichier `.jenga` est un script Python exécutable (cohérent avec le fait que Jenga est un DSL Python pur, sans génération de CMake/Makefile). |
| `# -*- coding: utf-8 -*-` | Déclaration d'encodage classique, utile si des commentaires/chaînes contiennent des caractères non-ASCII. |
| Docstring | Documentation humaine du module - pas interprétée par Jenga, sert à `jenga docs` et à la lecture du code. |
| `from Jenga import *` | Importe l'API DSL de Jenga : `project`, `filter`, `files`, `defines`, `usetoolchain`, etc. |
| `from jengaconfig import *` | **[?]** Importe un module **propre au dépôt Nkentseu** (pas un module standard de Jenga). Contient très probablement les constantes réutilisées dans tout le projet (`TC_WINDOWS`, noms de toolchains, helpers comme `nkentseudependson`). Le contenu exact de `jengaconfig.py` n'a pas été vu - à confirmer en l'ouvrant directement. |

---

## 2. Déclaration du projet

```python
with project("NKCore"):
    language("C++")
    cppdialect("C++20")
    location(".")
```

| Élément | Type / rôle |
|---|---|
| `with project("NKCore")` | Ouvre un **contexte de projet** nommé `NKCore` - c'est l'identifiant que l'on retrouve avec `jenga build --target NKCore`. |
| `language("C++")` | Fixe le langage de compilation du projet. |
| `cppdialect("C++20")` | Fixe le standard du compilateur (`-std=c++20` ou équivalent MSVC). |
| `location(".")` | Chemin racine du projet, relatif à l'emplacement du fichier `.jenga` (ici, le dossier courant). |

**Type de projet non déclaré explicitement ici** - pas de `staticlib()`, `sharedlib()`, `consoleapp()`, etc. **[?]** Cela laisse supposer soit une valeur par défaut définie par `jengaconfig` ou par `nkentseudependson` (voir plus bas), soit un type par défaut de Jenga (à vérifier - dans les exemples officiels, le type de binaire est toujours déclaré explicitement).

---

## 3. Dépendances

```python
    nkentseudependson(
        ["NKPlatform"],
        selfexport="NKCore",
        extra_includes=["src", "pch"],
    )
```

| Élément | Rôle |
|---|---|
| `nkentseudependson(...)` | **[?] Fonction non standard.** L'API Jenga officielle expose `dependson([...])` pour le graphe de dépendances inter-projets. `nkentseudependson` est visiblement un **wrapper maison** (défini dans `jengaconfig`) spécifique à l'écosystème Nkentseu, qui doit combiner plusieurs opérations : déclarer la dépendance vers `NKPlatform`, exposer les symboles publics de `NKCore` sous un nom (`selfexport`), et ajouter des dossiers d'include supplémentaires. Le comportement précis (résolution de liens, propagation transitive des includes, gestion du type de lib) reste à vérifier dans le code source de `jengaconfig.py`. |
| `["NKPlatform"]` | Liste des projets dont dépend `NKCore` - ici uniquement `NKPlatform` (types fixes, macros fondamentaux, cf. docstring). |
| `selfexport="NKCore"` | **[?]** Probablement le nom sous lequel `NKCore` s'exporte lui-même aux projets qui dépendront de lui à leur tour (propagation en chaîne des includes/libs). Mécanisme non documenté dans les sources consultées. |
| `extra_includes=["src", "pch"]` | Dossiers d'en-têtes supplémentaires ajoutés à la liste d'includes du projet, en plus de ceux hérités des dépendances - probablement pour permettre les includes internes (`#include "NKCore/..."`) et l'accès au header précompilé. |

---

## 4. Précompilation (PCH)

```python
    pchheader("pch/pch.h")
    pchsource("pch/pch.cpp")
```

| Élément | Rôle |
|---|---|
| `pchheader("pch/pch.h")` | Déclare le header précompilé du projet. Syntaxe héritée du monde Premake : la valeur est traitée comme une chaîne passée telle quelle au compilateur, pas comme un chemin résolu automatiquement. |
| `pchsource("pch/pch.cpp")` | Fichier source qui génère le `.pch`/`.gch` (nécessaire sous MSVC notamment). |

Ces deux lignes seront **désactivées plus bas** (mises à `""`) pour Android et HarmonyOS - voir section Toolchains.

---

## 5. Fichiers sources

```python
    files([
        "src/NKCore/**.cpp",
        "src/NKCore/**.h",
    ])
```

Motifs glob : inclut récursivement tous les `.cpp` et `.h` sous `src/NKCore/`. Rien de spécifique ici - comportement standard, cohérent avec les exemples officiels (`files(["src/**.cpp", "include/**.hpp"])`).

---

## 6. Répertoires de sortie

```python
    objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}")
```

| Token | Sens (par analogie avec la syntaxe Premake, reprise par Jenga) |
|---|---|
| `%{wks.location}` | Chemin racine du workspace. |
| `%{cfg.buildcfg}` | Nom de la configuration active (`Debug` / `Release`). |
| `%{cfg.system}` | Système cible (`windows`, `linux`, `macos`, `android`...). |
| `%{prj.name}` | Nom du projet courant (`NKCore`). |

**[?]** Ces tokens sont identiques à la syntaxe Premake5. Jenga revendique ne **pas** générer de fichiers Premake/CMake/Makefile - il faut donc supposer que Jenga a **réimplémenté son propre moteur de substitution de tokens** compatible avec cette syntaxe, plutôt que de déléguer à Premake. Ce point d'implémentation interne n'a pas été confirmé par la documentation consultée.

- `objdir` : dossier des fichiers objets intermédiaires, isolé par config/système/projet (permet des builds parallèles multi-plateformes sans collision).
- `targetdir` : dossier de sortie de la bibliothèque compilée (`NKCore` étant une lib, probablement statique - cf. point d'interrogation §2).

---

## 7. Filtre - variante UWP (Windows)

```python
    with filter("system:Windows && options:windows-runtime=uwp"):
        objdir("%{wks.location}/Build/Obj/%{cfg.buildcfg}-%{cfg.system}-uwp/%{prj.name}")
        targetdir("%{wks.location}/Build/Lib/%{cfg.buildcfg}-%{cfg.system}-uwp")
```

- `filter("system:Windows && options:windows-runtime=uwp")` : condition combinant le système cible et une **option custom** `windows-runtime` valant `uwp`. **[?]** `options:windows-runtime=uwp` n'apparaît dans aucun exemple officiel vu jusqu'ici - c'est probablement une option personnalisée déclarée quelque part au niveau du workspace (`option(...)` ou équivalent), à retrouver dans le fichier `.jenga` racine du workspace Nkentseu.
- Sous cette condition, les dossiers de sortie sont redirigés vers un suffixe `-uwp` pour ne pas écraser les artefacts Windows desktop classiques.

---

## 8. Sélection des toolchains par plateforme

```python
    with filter("system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne"):
        usetoolchain(TC_WINDOWS)
    with filter("system:UWP || system:Windows && options:windows-runtime=uwp"):
        usetoolchain("xbox-clang")
    with filter("system:macOS"):
        usetoolchain("clang-native")
    with filter("system:Android"):
        # Workaround: disable PCH on Android (NDK r27 + clang 18 + libc++)
        pchheader("")
        pchsource("")
        usetoolchain("android-ndk")
    with filter("system:HarmonyOS"):
        # PCH desactive (NDK OHOS clang, meme contrainte qu'Android)
        pchheader("")
        pchsource("")
        usetoolchain("ohos-ndk")
    with filter("system:Web"):
        usetoolchain("emscripten")
    with filter("system:XboxSeries || system:XboxOne"):
        usetoolchain("xbox-clang")
```

| Filtre | Toolchain appliquée | Remarque |
|---|---|---|
| Windows desktop (hors UWP, hors Xbox) | `TC_WINDOWS` | **[?]** Constante importée de `jengaconfig` - probablement le nom d'une toolchain MSVC préenregistrée (`"windows"` ou similaire). Valeur exacte non vue. |
| UWP (`system:UWP`) **ou** Windows+option `uwp` | `"xbox-clang"` | **[?] Point surprenant.** Le nom `xbox-clang` est réutilisé pour la cible UWP *et* pour Xbox plus bas - cela suggère que le toolchain Clang ciblant l'univers Xbox/UWP (souvent basé sur le même SDK Microsoft GDK/clang-cl) est partagé entre ces cibles. À confirmer dans la définition du toolchain (`with toolchain("xbox-clang", ...)`), non visible ici. |
| macOS | `"clang-native"` | Toolchain Clang natif Apple. |
| Android | `"android-ndk"` | Toolchain NDK Android. **PCH désactivé** juste avant (`pchheader("")` / `pchsource("")`) - commentaire explicite : incompatibilité connue entre NDK r27, clang 18 et libc++. |
| HarmonyOS | `"ohos-ndk"` | Même contrainte PCH que Android (NDK OpenHarmony basé sur une chaîne clang similaire). |
| Web | `"emscripten"` | Toolchain WASM (emsdk), cohérente avec le support "Production" annoncé pour Web/WASM. |
| XboxSeries / XboxOne | `"xbox-clang"` | Toolchain partagée avec UWP (voir remarque ci-dessus). Statut officiellement "Partial" pour Xbox (nécessite Microsoft GDK). |

**[?]** Toutes les chaînes `"xbox-clang"`, `"clang-native"`, `"android-ndk"`, `"ohos-ndk"`, `"emscripten"` sont des **noms de toolchains enregistrées ailleurs** dans le workspace (probablement via des blocs `with toolchain("nom", ...)` dans un fichier de configuration central du dépôt Nkentseu). Leur définition précise (compilateur, flags, sysroot) n'est pas visible dans ce fichier de module.

---

## 9. Configurations Debug / Release

```python
    with filter("config:Debug"):
        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
        optimize("Off")
        symbols(True)
    with filter("config:Release"):
        defines(["NDEBUG", "RELEASE", "NKENTSEU_RELEASE"])
        optimize("Speed")
        symbols(False)
```

Rien d'ambigu ici - schéma classique :

- **Debug** : macros de debug définies, optimisation désactivée, symboles de debug générés (`-g`/PDB).
- **Release** : macros de release, optimisation orientée vitesse (`-O2`/`-O3` ou équivalent), pas de symboles embarqués.

`NKENTSEU_DEBUG` / `NKENTSEU_RELEASE` sont des macros **spécifiques au framework** (probablement utilisées dans les headers NKCore pour du code conditionnel, asserts actifs uniquement en debug, etc.).

---

## 10. Tests unitaires

```python
    with filter("(system:Linux || system:macOS || (system:Windows && !options:windows-runtime=uwp && !system:XboxSeries && !system:XboxOne)) && !system:Android && !system:iOS || system:Web"):
        with test():
            testfiles(["tests/**.cpp"])
```

| Élément | Rôle |
|---|---|
| Condition du filtre | Longue expression booléenne : active les tests sur **Linux, macOS, Windows desktop (hors UWP/Xbox)**, en excluant explicitement Android et iOS, **plus** Web. |
| `with test():` | Contexte du framework de test intégré (**Unitest**), mentionné dans la doc Jenga comme fonctionnalité native (`jenga test`). |
| `testfiles(["tests/**.cpp"])` | Fichiers de test compilés dans la cible de test associée à `NKCore`. |

**[?] Précédence des opérateurs.** L'expression `A || B || (C) && !D && !E || F` repose sur la précédence implicite `&&` > `||` du mini-langage de filtre de Jenga (comme en Python/C). Elle est donc interprétée comme :
`A || B || (C && !D && !E) || F`
c'est-à-dire : *(Linux) OU (macOS) OU (Windows-desktop ET non-Android ET non-iOS) OU (Web)*.
Cette lecture suppose que le moteur de filtres de Jenga respecte bien cette précédence usuelle - à vérifier dans le parseur de filtres (`Jenga/Core/...`), non consulté ici. Si la précédence differe, le comportement réel pourrait diverger de cette lecture.

Autre point **[?]** : Android et iOS sont exclus alors qu'ils ne sont pas non plus couverts par les conditions positives (Linux/macOS/Windows/Web) - l'exclusion semble donc redondante avec la structure, sauf si elle sert à garder l'expression robuste si l'un des blocs positifs est modifié plus tard. Intention exacte de l'auteur non confirmée.

---

## Résumé des points marqués `[?]`

1. Contenu exact de `jengaconfig.py` (constantes, helpers).
2. Type de bibliothèque de `NKCore` non déclaré explicitement dans ce fichier (staticlib/sharedlib implicite ?).
3. Comportement précis de `nkentseudependson(...)` (wrapper non standard).
4. Sens exact de `selfexport="NKCore"`.
5. Mécanisme interne de substitution des tokens `%{...}` (repris de Premake mais implémenté nativement par Jenga ?).
6. Existence et définition de l'option custom `windows-runtime=uwp`.
7. Définition réelle des toolchains nommées (`xbox-clang`, `clang-native`, `android-ndk`, `ohos-ndk`, `emscripten`, `TC_WINDOWS`).
8. Réutilisation du toolchain `xbox-clang` à la fois pour UWP et pour Xbox - logique voulue ou coïncidence de nommage ?
9. Précédence exacte des opérateurs `&&`/`||` dans le mini-langage de `filter(...)`.