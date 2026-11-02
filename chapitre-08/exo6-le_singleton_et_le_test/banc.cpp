
// Exercice 6 : Le singleton et le test

#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKLogger/NkLog.h"
#include "NKInput/NkInput.h"
#include "NKFileSystem/NkFile.h"
#include <cstdio>
bool TestSautDirect() {
    if (nkentseu::NkInput.IsKeyDown(nkentseu::NkKey::NK_SPACE)) {
        return true;
    }
    return false;
}
bool TestSautLogique(bool auSol, bool barreEspaceEnfoncee) {
    return auSol && barreEspaceEnfoncee;
}
class BancEssai : public nkentseu::renderer::NkCanvasApp {
public:
    nkentseu::NkOptional<int> OnCommandLine(const nkentseu::NkVector<nkentseu::NkString> &args) override {
        nkentseu::NkFile logFile;
        logFile.Open("banc_resultats.txt", nkentseu::NkFileMode::Write);

        int echecs = 0;
        auto Verifier = [&](const char* nom, bool condition) {
            if (!condition) {
                nkentseu::NkString msg = nkentseu::NkPrintf("%s: ECHEC", nom);
                logFile.WriteLine(msg.CStr());
                echecs++;
            } else {
                nkentseu::NkString msg = nkentseu::NkPrintf("%s: OK", nom);
                logFile.WriteLine(msg.CStr());
            }
        };

        Verifier("Saut quand au sol et espace", TestSautLogique(true, true) == true);
        Verifier("Pas de saut quand en l'air et espace", TestSautLogique(false, true) == false);
        Verifier("Pas de saut quand au sol sans espace", TestSautLogique(true, false) == false);
        Verifier("Pas de saut quand en l'air sans espace", TestSautLogique(false, false) == false);

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
