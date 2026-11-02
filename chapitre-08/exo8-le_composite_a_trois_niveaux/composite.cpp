#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Core/NkTransformable.h"
#include "NKCanvas/Renderer/Core/NkDrawable.h"
#include "NKCanvas/Renderer/Core/NkRenderStates.h"
#include "NKFileSystem/NkFile.h"

class Noeud : public nkentseu::renderer::NkTransformable, public nkentseu::renderer::NkDrawable {
public:
    Noeud* enfant = nullptr;

    void Draw(nkentseu::renderer::NkRenderTarget& target, const nkentseu::renderer::NkRenderStates& states) const override {
        nkentseu::renderer::NkRenderStates local = states;
        local.transform *= GetTransform();
        if (enfant) enfant->Draw(target, local);
    }
};

class TestComposite : public nkentseu::renderer::NkCanvasApp {
    bool OnInit() override {
        mRoot = new Noeud();
        mRoot->SetPosition({100.f, 100.f});

        mLevel2 = new Noeud();
        mLevel2->SetPosition({50.f, 0.f});
        mLevel2->SetRotation(nkentseu::math::NkAngle(90.f));
        mRoot->enfant = mLevel2;

        mSprite = new Noeud();
        mSprite->SetPosition({0.f, 0.f});
        mLevel2->enfant = mSprite;

        mFile.Open("mesure.txt", nkentseu::NkFileMode::NK_WRITE);
        return true;
    }

    void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
        auto posSprite = mSprite->GetTransform().TransformPoint({0.f, 0.f});
        auto posFinale = mRoot->GetTransform().TransformPoint(posSprite);
        mFile.WriteLine((nkentseu::NkString("Position: (") + nkentseu::NkString::FromFloat(posFinale.x) + ", " + nkentseu::NkString::FromFloat(posFinale.y) + ")").CStr());
        Quit();
    }

private:
    Noeud* mRoot = nullptr;
    Noeud* mLevel2 = nullptr;
    Noeud* mSprite = nullptr;
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
