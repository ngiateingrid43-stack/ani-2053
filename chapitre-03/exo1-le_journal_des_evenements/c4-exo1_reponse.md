# c4-exo1 — Journal des événements

Premier exercice du chapitre 4 (fenêtrage et événements) avec le framework **Nkentseu** (`NKWindow`).

## Énoncé

Afficher dans le journal chaque événement reçu, avec sa **famille** et son **type**. Bouger la souris, taper au clavier, redimensionner la fenêtre, déposer un fichier. Compter combien d'événements produit une seconde d'usage normal.

## Objectifs pédagogiques

- Créer une fenêtre avec `NkWindow` et une `NkWindowConfig`.
- Comprendre la boucle d'événements : `NkEvents().PollEvent()` vide la file à chaque image.
- Classer les événements par catégorie (`NkEventCategory`).
- Mesurer le **volume** d'événements et en tirer des conséquences de conception.

## Compilation et exécution

Projet Nkentseu classique (Visual Studio / Jenga selon votre configuration). Lancer l'exécutable ; la console affiche le journal en continu. Fermer la fenêtre termine le programme (`WindowClose` → `window.Close()`).

## Fonctionnement du programme

1. **Configuration** : fenêtre 800×450, centrée, avec `cfg.dropEnabled = true` pour recevoir les dépôts de fichiers.
2. **Boucle principale** (`while (window.IsOpen())`) :
   - accumule le temps écoulé avec `NkClock::Tick().delta` ;
   - vide la file d'événements ; pour chacun, incrémente les compteurs et affiche une ligne :
     `[timestamp ms] famille=… type=… | description`
   - toutes les secondes, affiche un **bilan** : événements de la dernière seconde, maximum observé, total ;
   - dort 16 ms (`NkChrono::Sleep`) pour simuler ~60 images/s.
3. **Famille** : la fonction `Famille()` teste `HasCategory` dans l'ordre clavier, souris, manette, tactile ; tout le reste tombe dans « autre (fenêtre/système/dépôt…) ».

## Résultats observés

Exécution de 41,97 s, terminée normalement. Familles et types rencontrés :

| Famille | Type (id) | Événement | Remarque |
|---|---|---|---|
| souris | 38 | `MouseRaw(dx,dy)` | Déplacement **brut relatif**, de très loin le plus fréquent |
| souris | 37 | `MouseMove(x,y d=dx,dy)` | Position absolue, n'apparaît qu'à la réentrée dans la fenêtre |
| souris | 45 / 46 | `MouseEnter` / `MouseLeave` | Entrée / sortie du curseur |
| autre | 9 | `WindowPaint` | Demande de redessin |
| autre | 10 | `WindowResize` | Nouvelle taille |
| autre | 13 | `WindowMove` | Nouvelle position |
| autre | 17 | `WindowFocusLost` | Perte du focus |
| autre | 18 | `WindowMinimize` | Réduction |
| autre | 19 | `WindowMaximize` | Agrandissement |
| autre | 20 | `WindowRestore` | Restauration |
| autre | 7 / 8 | `WindowClose` / `WindowDestroy` | Fermeture |

> Les identifiants numériques dépendent de l'énumération `NkEventType` de la version du framework utilisée ; ne pas les coder en dur.

### Volume d'événements

Pendant un mouvement rapide de souris, le journal montre **environ 300 événements `MouseRaw` en 16 ms** (timestamps 44469817 → 44469833), soit bien plus qu'un événement par image. Pendant les pauses (fenêtre inactive, curseur immobile), il n'y en a quasiment aucun.

Conclusion : le nombre d'événements par seconde n'est **pas constant**. Il dépend surtout de la souris (et de sa fréquence d'interrogation) ; clavier, fenêtre et dépôts de fichiers représentent quelques événements ponctuels.

À compléter avec vos propres mesures (lignes `=== … ===` du programme) :

| Situation | Événements / seconde |
|---|---|
| Souris immobile | |
| Souris en mouvement lent | |
| Souris en mouvement rapide | |
| Saisie clavier | |
| Redimensionnement | |
| Dépôt d'un fichier | |

## Observations intéressantes

- **Réduction de la fenêtre** : on reçoit `WindowMove(33536,33536)` puis `WindowResize(0x0)`. 33536 correspond à −32000 lu comme entier non signé 16 bits : Windows déplace la fenêtre « hors écran » quand elle est réduite. Il faut ignorer ces valeurs (ne pas redimensionner les buffers à 0×0).
- **Horodatages non monotones** : `WindowMaximize` (44477197) est affiché avant `WindowPaint` (44477166). L'ordre de livraison n'est donc pas strictement l'ordre des timestamps ; ne pas s'appuyer sur le tri par temps.
- **Rafale au retour de focus** : après la restauration, un nouveau paquet de `MouseRaw` arrive d'un coup (mouvements accumulés).
- **Coût du journal lui-même** : `printf` par événement dans une console est lent ; à fort débit, il fausse la mesure et ralentit la boucle.

## Leçons à retenir

1. Ne jamais faire de travail lourd **par événement** de souris brute : agréger (somme des `dx,dy`) et appliquer une seule fois par image.
2. Utiliser `MouseMove` (absolu) pour l'interface, `MouseRaw` (relatif) pour les caméras et les jeux.
3. Toujours **vider entièrement** la file (`while (PollEvent())`) à chaque image, sinon elle grossit et l'entrée devient en retard.
4. Filtrer les cas limites (fenêtre réduite, taille 0×0).
5. Pour mesurer proprement, compter d'abord, n'afficher que le bilan.
