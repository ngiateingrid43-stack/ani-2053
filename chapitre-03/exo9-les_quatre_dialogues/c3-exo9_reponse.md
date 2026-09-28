# Exercice 9 — Les quatre dialogues

## Traitement de l'annulation

Écart avec l'énoncé : « employez les quatre dialogues... et traitez correctement l'annulation dans chacun » suppose que chacun peut être annulé et détecté. Ce n'est vrai que pour 3 des 4 : OpenFileDialog, SaveFileDialog et OpenFolderDialog renvoient confirmed = false si l'utilisateur ferme sans choisir — testé avant de lire path. OpenMessageBox ne renvoie rien : il n'y a pas de bouton Oui/Non/Annuler dans cette implémentation, donc rien à tester côté annulation. Ce que j'ai vérifié à la place : fermer la boîte de message (croix ou touche) ne fait pas planter le programme.

## Resultat

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  window.exe
     C:\Users\DELL\Desktop\FirstWindow\FirstWindow\Build\Bin\Debug-Windows\window\window.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

A = message, B = ouvrir, C = enregistrer, D = dossier. Fermez chaque boite sans choisir pour tester l'annulation.
message : ferme sans planter (aucune valeur de retour a lire, voir commentaire en tete de fichier)
ouvrir : ANNULE (aucun fichier)
dossier : ANNULE (aucun dossier)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (44.85s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
PS C:\Users\DELL\Desktop\FirstWindow\FirstWindow> 
```
