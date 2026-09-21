# Exo 12 — La règle du dépôt

*Règles git pour un groupe de quatre étudiants — applicables dès demain.*

## 1. Nommage des branches

- `main` : toujours stable, toujours dans un état qui compile et qui
  tourne. Personne n'y commit directement.
- `feature/<nom-court>` : une branche par fonctionnalité, un seul auteur
  principal (ex. `feature/inventaire-joueur`).
- `fix/<nom-court>` : correctif ciblé, hors fonctionnalité en cours.
- Pas de branche vide plus de 5 jours : soit elle avance, soit elle est
  supprimée.

## 2. Contenu d'un commit

- Un commit = un sujet. Si le message a besoin d'un "et", c'est deux
  commits.
- Message au format : `type: résumé court à l'impératif`
  (`feat`, `fix`, `refactor`, `docs`, `perf`, `test`).
- Le corps du message explique le **pourquoi**, pas seulement le quoi (le
  diff montre déjà le quoi).
- Un commit ne casse jamais la compilation à lui seul.

## 3. Qui relit quoi

- Toute branche `feature/*` ou `fix/*` passe par une pull request avant
  fusion dans `main`.
- Relecture croisée obligatoire : l'auteur ne fusionne jamais sa propre
  PR.
- Au moins une approbation avant fusion ; en cas de désaccord persistant,
  on tranche à quatre en 10 minutes maximum, pas par échange de messages
  qui traîne.

## 4. Ce qui est interdit

- `git push --force` sur `main`, sous aucun prétexte.
- Commiter des fichiers générés, des identifiants/secrets, ou des
  fichiers binaires lourds sans accord du groupe.
- Réécrire l'historique d'une branche que quelqu'un d'autre a déjà tirée.
- Laisser une branche de fonctionnalité vivre plus d'une semaine sans la
  rebaser/fusionner sur `main` (les divergences trop longues produisent
  des conflits ingérables).

## 5. Quand quelqu'un casse `main`

1. On arrête tout nouveau merge dans `main` immédiatement.
2. Celui qui a cassé la branche a la priorité pour corriger — sinon,
   `git revert` du commit fautif sans discussion, pour rétablir un état
   stable en premier, discuter ensuite.
3. Un correctif (`revert` ou `fix`) doit rétablir `main` en moins de
   30 minutes après signalement.
4. Post-mortem rapide à quatre : qu'est-ce qui n'a pas été vu en revue ?
   Ajuste-t-on une règle ci-dessus en conséquence ?

Ces règles tiennent sur une page volontairement : elles doivent pouvoir
être relues et appliquées dès le prochain commit du groupe.
