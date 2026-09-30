# c4-exo5 — Un clic consommé par un panneau

Cinquième exercice du chapitre 4 (fenêtrage et événements) avec le framework **Nkentseu** (`NKWindow`).

## Fonctionnement du programme

1. Fenêtre de 800×500 ; le **panneau** est le rectangle en haut à gauche : origine (0,0), taille 220×160.
2. `DansPanneau(x, y)` teste l'appartenance au rectangle (bord gauche et haut inclus, bord droit et bas exclus).
3. Deux callbacks sont enregistrés sur `NkMouseButtonPressEvent`, dans cet ordre :
   - **Panneau** (premier appelé, il est « au-dessus ») : si le clic est dans le rectangle, il affiche `[panneau] clic (x,y) CONSOMME` et appelle `e->MarkHandled()` ;
   - **Scène** (second, « en dessous ») : si `e->IsHandled()` est vrai, il affiche un message d'avertissement et ignore le clic ; sinon il affiche `[scene] clic (x,y) recu`.
4. La boucle appelle `PollEvent()`, ce qui « pompe » la file et déclenche les callbacks.

## Résultats

Exécution de 128,32 s terminée normalement. Bilan du journal :

| Catégorie | Nombre |
|---|---|
| Clics dans le panneau, consommés (`[panneau] ... CONSOMME`) | 5 |
| Messages `[scene] clic deja consomme, ignore` | 5 |
| Clics hors panneau reçus par la scène (`[scene] ... recu`) | 10 |
| Clics sur un point de la zone du panneau reçus par la scène (`recu`) | **0** |

Clics dans le panneau : (72,37), (48,47), (1,28), (0,44), (29,89).
Clics hors panneau : (795,53), (191,173), (686,211), (5,239), (791,128), (695,151), (586,119).

Deux cas limites utiles ont été testés :

- **(191,173)** : x est bien dans la largeur du panneau (< 220) mais y = 173 dépasse la hauteur (≥ 160) : le clic est correctement traité comme **extérieur**.
- **(5,239)** : même idée, x très petit mais y trop grand : extérieur.
- **(0,44)** et **(1,28)** : sur le bord gauche : correctement **inclus** dans le panneau.

## Analyse : le second gestionnaire est bien appelé

C'est le résultat le plus important de l'exercice, et il ne correspond pas à l'attente de l'énoncé : à chaque clic consommé, le journal montre `[scene] clic deja consomme, ignore (le systeme l'a quand meme livre)`.

- Le second callback **est donc appelé** malgré `MarkHandled()`.
- `MarkHandled()` ne **stoppe pas** la distribution : elle pose un **drapeau** sur l'événement. La consommation est une convention **coopérative** : chaque gestionnaire suivant doit lire `IsHandled()` et se retirer de lui-même.
- Le « filet » écrit dans le second callback (`if (e->IsHandled()) return;`) est ce qui produit le comportement voulu : aucun clic de la zone du panneau n'a été traité par la scène (0 ligne `recu`).

Le résultat attendu, « le second gestionnaire n'est pas appelé », n'est donc obtenu que **fonctionnellement** (la scène n'agit pas), pas **mécaniquement** (la scène est bel et bien invoquée).

## Ce que confirme le journal

- **Ordre d'appel = ordre d'inscription** : chaque ligne `[scene] ... ignore` suit immédiatement une ligne `[panneau] ... CONSOMME`, jamais l'inverse.
- Chaque clic dans le panneau produit **exactement deux** lignes (`CONSOMME` puis `ignore`) ; il n'y a jamais de doublon côté panneau.
- Trois clics hors panneau apparaissent **en double avec exactement les mêmes coordonnées** : (795,53), (191,173) et (791,128). Le journal ne permet pas de trancher : il peut s'agir de deux clics rapides au même pixel (double-clic), ou d'un événement livré deux fois. Comme aucun clic du panneau n'est doublé, la première explication est plus probable, mais c'est à confirmer.

