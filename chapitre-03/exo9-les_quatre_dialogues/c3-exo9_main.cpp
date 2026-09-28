// c3-exo9 : les quatre dialogues natifs (API reelle : NkDialogs, voir NKWindow/Core/NkDialogs.h).
// A = message (info/warning/error)   B = ouvrir un fichier   C = enregistrer sous   D = choisir un dossier.
// Annulation : chaque resultat expose "confirmed" (bool) ; on ne touche jamais a "path" sans l'avoir teste.
//
// IMPORTANT (constate a la compilation) : OpenMessageBox() est void, sans bouton Oui/Non/Annuler ni retour :
// on ne peut donc PAS tester une annulation sur la boite de message elle-meme avec cette API.
// Ce que je verifie a la place : fermer la boite (X ou touche) ne fait planter ni bloquer le programme.
#include "NKWindow/NKWindow.h"
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkDialogs.h"
#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c3-exo9";
    d.appVersion = "1.0.0";
    return d;
})());

int nkmain(const NkEntryState& state) {
    (void)state;
    NkWindowConfig cfg;
    cfg.title = "c3-exo9 - touches A a D"; cfg.width = 640; cfg.height = 360; cfg.centered = true;
    NkWindow window;
    if (!window.Create(cfg)) return -1;
    std::printf("A = message, B = ouvrir, C = enregistrer, D = dossier. Fermez chaque boite sans choisir pour tester l'annulation.\n");

    while (window.IsOpen()) {
        while (NkEvent* ev = NkEvents().PollEvent()) {
            if (ev->Is<NkWindowCloseEvent>()) { window.Close(); continue; }
            auto* kp = ev->As<NkKeyPressEvent>();
            if (!kp) continue;
            switch (kp->GetKey()) {
                case NkKey::NK_ESCAPE:
                    window.Close();
                    break;
                case NkKey::NK_A:
                    // Pas de choix Oui/Non/Annuler dans cette API : on verifie juste que fermer la boite ne plante pas.
                    NkDialogs::OpenMessageBox("Ceci est une boite de message.", "Exo9", 0);
                    std::printf("message : ferme sans planter (aucune valeur de retour a lire, voir commentaire en tete de fichier)\n");
                    break;
                case NkKey::NK_B: {
                    const NkDialogResult r = NkDialogs::OpenFileDialog("*.*", "Ouvrir un fichier");
                    if (r.confirmed) std::printf("ouvrir : %s\n", r.path.CStr());
                    else              std::printf("ouvrir : ANNULE (aucun fichier)\n");
                    break;
                }
                case NkKey::NK_C: {
                    const NkDialogResult r = NkDialogs::SaveFileDialog("txt", "Enregistrer sous");
                    if (r.confirmed) std::printf("enregistrer : %s\n", r.path.CStr());
                    else              std::printf("enregistrer : ANNULE (rien ecrit)\n");
                    break;
                }
                case NkKey::NK_D: {
                    const NkDialogResult r = NkDialogs::OpenFolderDialog("Choisir un dossier");
                    if (r.confirmed) std::printf("dossier : %s\n", r.path.CStr());
                    else              std::printf("dossier : ANNULE (aucun dossier)\n");
                    break;
                }
                default: break;
            }
        }
        NkChrono::Sleep((int64)16);
    }
    return 0;
}