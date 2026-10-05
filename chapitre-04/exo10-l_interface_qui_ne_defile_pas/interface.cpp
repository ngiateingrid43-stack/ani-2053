#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Shapes/NkRectangleShape.h"
#include "NKLogger/NkLog.h"
#include "NKWindow/NKMain.h"

using namespace nkentseu;
using namespace nkentseu::renderer;

constexpr bool kReset = true;

static NkView2D CreerVue(float32 cx, float32 cy, float32 w, float32 h) {
	NkView2D v;
	v.center = NkVec2f{cx, cy};
	v.size = NkVec2f{w, h};
	return v;
}

class Interface : public NkCanvasApp {
	public:
		Interface() : mCarre({60.f, 60.f}), mBarre({960.f, 50.f}) {
			Config().title = "Interface";
			Config().width = 960;
			Config().height = 540;
			Config().clearColor = NkColor2D{20, 67, 23, 255};
		}

	protected:
		bool OnInit() override {
			mBarre.SetPosition({0.f, 0.f});
			mBarre.SetFillColor({30, 30, 40, 255});
			return true;
		}

		void OnUpdate(float32 dt) override {
			mCentreX += kVitesse * dt;
			mTemps += dt;
		}

		void OnRender(NkRenderWindow &target) override {
			const math::NkVec2u taille = target.GetSize();
			const float32 largeur = static_cast<float32>(taille.x);
			const float32 hauteur = static_cast<float32>(taille.y);

			target.SetView(CreerVue(mCentreX, hauteur * 0.5f, largeur, hauteur));

			for (int32 i = 0; i < kNbCarres; ++i) {
				mCarre.SetPosition({static_cast<float32>(i) * kPas, 240.f});
				mCarre.SetFillColor({static_cast<uint8>(40 + (i % 40) * 5), 150, 220, 255});
				target.Draw(mCarre);
			}

			if constexpr (kReset) {
				target.ResetView();
			}

			const NkVec2i pixelBarre = target.MapCoordsToPixel(NkVec2f{0.f, 0.f});
			const bool vueActive = target.IsViewCustom();

			mBarre.SetSize({largeur, 50.f});
			target.Draw(mBarre);

			if (mTemps >= mProchaineLigne) {
				mProchaineLigne += 1.f;
			}
		}

	private:
		static constexpr float32 kVitesse = 200.f;
		static constexpr int32 kNbCarres = 40;
		static constexpr float32 kPas = 120.f;

		NkRectangleShape mCarre;
		NkRectangleShape mBarre;

		float32 mCentreX = 480.f;
		float32 mTemps = 0.f;
		float32 mProchaineLigne = 1.f;
};

int nkmain(const NkEntryState &state) {
	return NkCanvasApp::Run<Interface>(state);
}