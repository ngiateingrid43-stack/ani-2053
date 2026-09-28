# Exercice 8 — Le presse-papiers, dans les deux sens — réponse

## Texte

`GetClipboardText()` → `NkString::ToUpper()` (en place) → `SetClipboardText()`.
Sous Win32, c'est le vrai presse-papiers du système : coller ensuite dans le Bloc-notes
montre le texte en majuscules. Sur les autres plateformes, le presse-papiers texte est
un repli interne à l'application (copier/coller *intra*-app uniquement) — coller dans
une autre application ne verra donc pas le résultat.

## Image

`HasClipboardImage()` (test léger, sans copier les pixels) → `GetClipboardImage(out)`
→ boucle sur `out.pixels` (RGBA8, 4 octets par pixel) inversant R, G, B (`255 - v`) et
laissant l'alpha intact → `SetClipboardImage(out)`.

- Sous Win32 : lit `CF_DIBV5` puis `CF_DIB` (24 et 32 bits). Une capture d'écran
  (touche Impr. écran) ou une image copiée depuis un navigateur fonctionnent. Une
  source qui ne pose **qu'un PNG** (sans DIB) n'est pas lue : décoder du PNG
  demanderait NKImage, que NKWindow ne tire volontairement pas.
- Sur les autres plateformes : pas encore d'implémentation OS (X11 CLIPBOARD image/png,
  NSPasteboard, wl_data_device sont notés dans la feuille de route) — `HasClipboardImage()`
  renvoie `false` proprement, sans planter.

## Pourquoi `NkClipboardImage` et pas `NkImage`

NKWindow ne dépend pas de NKImage : le fenêtrage n'a pas à tirer les douze codecs
d'image pour une opération aussi ponctuelle. Une application qui veut un vrai `NkImage`
recopie simplement les pixels bruts (`NkImage::Create` + `memcpy`).

## Resultat de L'exution

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ▶  EXECUTION  —  window.exe
     C:\Users\DELL\Desktop\FirstWindow\FirstWindow\Build\Bin\Debug-Windows\window\window.exe
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Copie un texte puis appuie sur T. Copie une image (capture d'ecran) puis appuie sur I.
[texte] avant : "NkClipboardImage"
[texte] apres : "NKCLIPBOARDIMAGE" (remis dans le presse-papiers)
[texte] avant : "NKCLIPBOARDIMAGE"
[texte] apres : "NKCLIPBOARDIMAGE" (remis dans le presse-papiers)
[image] pas d'image dans le presse-papiers (copie une capture d'ecran, puis reessaie).
[image] pas d'image dans le presse-papiers (copie une capture d'ecran, puis reessaie).
[image] pas d'image dans le presse-papiers (copie une capture d'ecran, puis reessaie).

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ◀  FIN D'EXECUTION  —  termine normalement  (19.55s)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```