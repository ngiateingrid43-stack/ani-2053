#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCore/NkOptional.h"
#include "NKFileSystem/NkFile.h"

// Version directe : lit le singleton (non testable sans le kit complet)
// NkInput n'existe pas dans le kit (include/), donc on ne peut pas la compiler.
// Voir la version testable ci-dessous.

bool TestSautLogique(bool auSol, bool espace) {
    return auSol && espace;
}

class BancEssai : public nkentseu::renderer::NkCanvasApp {
public:
    nkentseu::NkOptional<int> OnCommandLine(const nkentseu::NkVector<nkentseu::NkString> &args) override {
        nkentseu::NkFile logFile;
        logFile.Open("banc_resultats.txt", nkentseu::NkFileMode::NK_WRITE);

        int echecs = 0;
        auto Verifier = [&](const char* nom, bool condition) {
            if (!condition) {
                echecs++;
                logFile.WriteLine((nkentseu::NkString(nom) + ": ECHEC").CStr());
            } else {
                logFile.WriteLine((nkentseu::NkString(nom) + ": OK").CStr());
            }
        };

        Verifier("Saut quand au sol et espace", TestSautLogique(true, true));
        Verifier("Pas de saut en l'air avec espace", !TestSautLogique(false, true));
        Verifier("Pas de saut au sol sans espace", !TestSautLogique(true, false));
        Verifier("Pas de saut en l'air sans espace", !TestSautLogique(false, false));

        logFile.Close();
        return echecs;
    }
};

int nkmain(const nkentseu::NkEntryState &state) {
    return nkentseu::renderer::NkCanvasApp::Run<BancEssai>(state);
}

NKENTSEU_DEFINE_APP_DATA([]{
    nkentseu::NkAppData d{};
    d.appName = "BancTest";
    d.appVersion = "1.0.0";
    return d;
});
