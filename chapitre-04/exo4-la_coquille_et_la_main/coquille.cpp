// Version avec la coquille : la fenetre, la cible, l'horloge et la boucle sont
// montees par NkCanvasApp. On ne remplit que trois choses.
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName    = "c4-exo4-coquille";
    d.appVersion = "1.0.0";
    return d;
})());

namespace {
constexpr float32 LARGEUR = 800.f;
constexpr float32 HAUTEUR = 450.f;
constexpr float32 COTE    = 50.f;
constexpr float32 VITESSE = 100.f; // pixels par seconde
}

class CarreRouge : public NkCanvasApp {
    public:
        CarreRouge() {
            // La configuration se pose dans le constructeur, avant la fenetre.
            Config().title     = "c4-exo4 - coquille";
            Config().width     = static_cast<uint32>(LARGEUR);
            Config().height    = static_cast<uint32>(HAUTEUR);
            Config().resizable = false;
            Config().clearColor = NkColor2D{18, 18, 24, 255};
        }

    protected:
        void OnUpdate(float32 dt) override {
            mX += VITESSE * dt;
            if (mX > LARGEUR) mX = -COTE;
        }

        void OnRender(NkRenderWindow &target) override {
            // Clear et Display sont faits par la coquille, avant et apres.
            target.GetRenderer2D().DrawFilledRect({mX, (HAUTEUR - COTE) * 0.5f, COTE, COTE}, NkColor2D::Red);
        }

    private:
        float32 mX = 0.f;
};

int nkmain(const NkEntryState &state) {
    return renderer::NkCanvasApp::Run<CarreRouge>(state);
}