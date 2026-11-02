
// Exercice 8 : Le composite à trois niveaux

#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKLogger/NkLog.h"
#include "NKFileSystem/NkFile.h"

class Noeud : public nkentseu::renderer::NkTransformable, public nkentseu::renderer::NkDrawable {
public:
    Noeud* enfant = nullptr;

    void Draw(nkentseu::renderer::NkRenderTarget& target, const nkentseu::renderer::NkRenderStates& states) const override {
        nkentseu::renderer::NkRenderStates local = states;
        local.transform *= GetTransform();
        if (enfant) {
            enfant->Draw(target, local);
        }
    }
};

class SondeDefectueuse : public nkentseu::renderer::NkTransformable, public nkentseu::renderer::NkDrawable {
public:
    void Draw(nkentseu::renderer::NkRenderTarget& target, const nkentseu::renderer::NkRenderStates& states) const override {
        nkentseu::renderer::NkRenderStates local;
        local.transform = GetTransform();
        // Jette states.transform : défaut imité
    }
};

class TestComposite : public nkentseu::renderer::NkCanvasApp {
    bool OnInit() override {
        mRoot = new Noeud();
        mRoot->SetPosition({100, 100});

        mLevel2 = new Noeud();
        mLevel2->SetPosition({50, 0});
        mLevel2->SetRotation(90.f);
        mRoot->enfant = mLevel2;

        mSprite = new Noeud();
        mSprite->SetPosition({0, 0});
        mLevel2->enfant = mSprite;

        mFile.Open("mesure.txt", nkentseu::NkFileMode::Write);
        return true;
    }

    void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
        nkentseu::math::NkVec2f posInitiale = mSprite->GetTransform().transformPoint({0, 0});
        nkentseu::math::NkVec2f posFinale = mRoot->GetTransform().transformPoint(posInitiale);

        mFile.WriteLine(nkentseu::NkPrintf("Position mesurée: (%f, %f)", posFinale.x, posFinale.y).CStr());
        this->Quit();
    }
private:
    Noeud* mRoot;
    Noeud* mLevel2;
    Noeud* mSprite;
    nkentseu::NkFile mFile;
};

int nkmain(const nkentseu::NkEntryState &state) {
    return nkentseu::renderer::NkCanvasApp::Run<TestComposite>(state);
}

NKENTSEU_DEFINE_APP_DATA([]{
    nkentseu::NkAppData d{};
    d.appName = "TestComposite";
    d.appVersion = "1.0.0";
    return d;
});
