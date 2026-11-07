#include "NkRef/NkRefView.h"

int main() {
	const nkref::NkRefView vue{};
	const nkref::NkVec2f pixel{12.0f, 30.0f};
	const nkref::NkVec2f viewport{800.0f, 600.0f};
	const nkref::NkVec2f world = vue.PixelToWorld(pixel, viewport);
	const nkentseu::float32 spacing = vue.GridSpacing(24.0f);
	// vue.PanByPixels(1.0f, 0.0f);
	(void)world;
	(void)spacing;
	return 0;
}
