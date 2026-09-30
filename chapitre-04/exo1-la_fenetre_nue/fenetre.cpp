#include "NKCanvas/App/NkCanvasApp.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

class FenetreNue : public NkCanvasApp {
public:
    FenetreNue() {
        Config().title = "La fenetre nue";
        Config().width = 800;
        Config().height = 450;
        Config().clearColor = NkColor2D{ 32, 40, 56, 255 };
    }
};

int nkmain(const NkEntryState& state) {
    return NkCanvasApp::Run<FenetreNue>(state);
}