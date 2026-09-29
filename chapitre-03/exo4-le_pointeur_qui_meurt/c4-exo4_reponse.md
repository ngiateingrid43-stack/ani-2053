# c4-exo4 — Ne gardez pas un pointeur d'événement

## Fonctionnement du programme

Le programme a deux modes, on passe du premier au second en appuyant sur **C**.

**Mode BUG** (au démarrage) :

1. `PollEvent()` renvoie un pointeur brut ; le programme le mémorise dans `garde`.
2. Il copie aussi le texte de l'événement dans `instantane` (la « vérité » lue à la réception).
3. Après la boucle, il relit `garde->ToString()` et compare avec `instantane` : `identique` ou `!!! CONTENU CHANGE / pointeur perime`.

**Mode CORRIGÉ** :

1. `PollEventCopy()` renvoie un `NkEventPtr` que l'on déplace dans `copie` avec `std::move`.
2. La relecture doit toujours afficher `identique (sur)`.

> Correction de compilation : `NkMove` de Nkentseu est un équivalent de `memmove` (3 ou 7 arguments), pas de `std::move`. Il faut utiliser `std::move` (`#include <utility>`).

## Résultats observés

Exécution de 28,57 s terminée normalement, **uniquement en mode bug** (aucune ligne `--- mode CORRIGE ---` : la touche C n'a pas été pressée).

Nombre d'événements par trame :

| Situation | Événements / trame |
|---|---|
| Souris immobile | 0 (la plupart des trames au repos) |
| Souris en mouvement | environ 5 à 15 |
| Pics ponctuels | 23 à 48, avec deux rafales à 187 et 188 |
| Première trame | 27 |

Contenu du dernier événement conservé : le plus souvent `MouseRaw(0,0)`, parfois un `MouseMove(x,y d=dx,dy)`, plus rarement `MouseLeave()` ou `WindowFocusGained()`.

**Toutes les lignes affichent `identique`.** Le message `!!! CONTENU CHANGE / pointeur perime` n'apparaît jamais.

## Analyse

### Le bug n'a pas été observé, et ce n'est pas une preuve qu'il n'existe pas

Un comportement indéfini peut très bien « fonctionner » : ici, l'événement pointé est resté lisible et inchangé. Cela indique que la file conserve le dernier événement retourné tant qu'aucun nouvel appel à `PollEvent()` ne le remplace. Un bug possible n'apparaît donc que dans les conditions où la file recycle vraiment l'emplacement mémoire.

### Le test tel qu'il est écrit ne peut pas révéler le problème

Le programme ne conserve que le **dernier** événement de la trame (`garde = ev` à chaque tour) et le relit juste après la boucle, avant tout nouvel appel à `PollEvent()`. C'est pour cela que la comparaison est toujours vraie, y compris quand une trame contient des centaines d'événements.

Pour provoquer le cas « deux événements dans la même trame » demandé par l'énoncé, il faut garder le pointeur du **premier** événement, puis lire les suivants :

```cpp
NkEvent* premier = nullptr;
char texteInitial[512] = "";
int dansLaTrame = 0;

while (NkEvent* ev = NkEvents().PollEvent()) {
    ++dansLaTrame;
    if (dansLaTrame == 1) {                       // on garde le PREMIER
        premier = ev;
        std::snprintf(texteInitial, sizeof(texteInitial), "%s", ev->ToString().CStr());
    }
}
if (premier && dansLaTrame >= 2) {                // au moins deux evenements dans la trame
    const NkString relu = premier->ToString();
    const bool pareil = std::strcmp(relu.CStr(), texteInitial) == 0;
    std::printf("%d evt(s) | premier->\"%s\" | %s\n", dansLaTrame, relu.CStr(),
                pareil ? "identique" : "!!! CONTENU CHANGE");
}
```
### Le cas « plusieurs événements par trame » est la norme

Vos mesures montrent qu'une trame contient rarement un seul événement : 5 à 15 pendant un mouvement, jusqu'à 188 en rafale. Un programme qui suppose « un événement par trame » ou qui garde un pointeur est donc fragile dès qu'on bouge la souris.
