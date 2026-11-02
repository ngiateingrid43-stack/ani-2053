
// Exercice 4 : Les crochets du patron de méthode

#include "NKWindow/NKMain.h"
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKLogger/NkLog.h"
#include <cstdio>

class MonJeu : public nkentseu::renderer::NkCanvasApp {
public:
    bool OnInit() override {
        mJournal = fopen("journal_crochets.txt", "w");
        if (!mJournal) {
            nkentseu::logger.Error("Impossible d'ouvrir le journal.");
            return false;
        }
        fprintf(mJournal, "1,OnInit,%d,%f\n", 0, nkentseu::NkTime::Time());
        return true;
    }

    void OnUpdate(float32 deltaTime) override {
        if (mFrameCount < 5) {
            fprintf(mJournal, "2,OnUpdate,%d,%f\n", mFrameCount, nkentseu::NkTime::Time());
        }
        mFrameCount++;
    }

    void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
        if (mFrameCount < 5) {
            fprintf(mJournal, "3,OnRender,%d,%f\n", mFrameCount, nkentseu::NkTime::Time());
        }
        if (mFrameCount > 10) {
            this->Quit();
        }
    }

    void OnLayout(const nkentseu::NkLayoutInfo &layout) override {
        fprintf(mJournal, "4,OnLayout,%d,%f\n", 0, nkentseu::NkTime::Time());
    }

    void OnShutdown() override {
        fprintf(mJournal, "5,OnShutdown,%d,%f\n", 0, nkentseu::NkTime::Time());
        fclose(mJournal);
    }

private:
    FILE* mJournal = nullptr;
    int mFrameCount = 0;
};

int nkmain(const nkentseu::NkEntryState &state) {
    return nkentseu::renderer::NkCanvasApp::Run<MonJeu>(state);
}

NKENTSEU_DEFINE_APP_DATA([]{
    nkentseu::NkAppData d{};
    d.appName = "TestCrochets";
    d.appVersion = "1.0.0";
    return d;
});
