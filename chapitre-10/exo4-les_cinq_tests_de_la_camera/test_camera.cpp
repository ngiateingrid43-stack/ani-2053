// Compilation, la caméra cassée d'abord dans le chemin d'inclusion :
//   g++ -std=c++17 -Wall test_camera.cpp -o test_camera -Icopie -I<racine moteur>

#include "NkRef/NkRefView.h"

#include <cstdio>

namespace {

const double kTolerance = 0.001;

bool Proche(double a, double b) {
        const double d = a - b;
        return (d < 0 ? -d : d) <= kTolerance;
}

nkref::NkRefView Camera() {
        nkref::NkRefView v;
        return v;
}

bool aller_retour() {
        nkref::NkRefView v = Camera();
        const nkentseu::math::NkVec2f viewport{800.0f, 600.0f};
        const nkentseu::math::NkVec2f pixels[5] = {{0.0f, 0.0f}, {800.0f, 600.0f}, {400.0f, 300.0f},
                                                                                                 {-37.5f, 912.25f}, {1234.0f, -56.0f}};
        v.center = {17.5f, -80.25f};
        v.zoom = 2.75f;
        for (int i = 0; i < 5; ++i) {
                const nkentseu::math::NkVec2f monde = v.PixelToWorld(pixels[i], viewport);
                const nkentseu::math::NkVec2f retour = v.WorldToPixel(monde, viewport);
                if (!Proche(retour.x, pixels[i].x) || !Proche(retour.y, pixels[i].y))
                        return false;
        }
        return true;
}

bool point_fixe() {
        nkref::NkRefView v = Camera();
        const nkentseu::math::NkVec2f viewport{1280.0f, 720.0f};
        const nkentseu::math::NkVec2f curseur{913.0f, 244.0f};
        v.center = {-42.0f, 13.0f};
        v.zoom = 1.5f;
        const nkentseu::math::NkVec2f avant = v.PixelToWorld(curseur, viewport);
        v.ZoomAtPixel(3.0f, curseur, viewport);
        const nkentseu::math::NkVec2f apres = v.PixelToWorld(curseur, viewport);
        return Proche(avant.x, apres.x) && Proche(avant.y, apres.y);
}

bool bornes() {
        nkref::NkRefView v = Camera();
        const nkentseu::math::NkVec2f viewport{800.0f, 600.0f};
        const nkentseu::math::NkVec2f curseur{400.0f, 300.0f};
        v.ZoomAtPixel(1000.0f, curseur, viewport);
        if (v.zoom > nkref::NkRefView::kZoomMax)
                return false;
        if (v.zoom < nkref::NkRefView::kZoomMin)
                return false;
        v.ZoomAtPixel(0.00001f, curseur, viewport);
        if (v.zoom > nkref::NkRefView::kZoomMax)
                return false;
        if (v.zoom < nkref::NkRefView::kZoomMin)
                return false;
        return true;
}

bool grille() {
        nkref::NkRefView v = Camera();
        const float m = 32.0f;
        v.zoom = 8.0f;
        float precedent = v.GridSpacing(m);
        for (int i = 0; i < 60; ++i) {
                v.zoom *= 0.5f;
                if (v.zoom < nkref::NkRefView::kZoomMin)
                        v.zoom = nkref::NkRefView::kZoomMin;
                const float pas = v.GridSpacing(m);
                if (pas < precedent)
                        return false;
                if (pas * v.zoom < m - 0.0001f)
                        return false;
                precedent = pas;
        }
        return true;
}

bool origine() {
        nkref::NkRefView v = Camera();
        const nkentseu::math::NkVec2f viewport{1024.0f, 768.0f};
        v.ZoomAtPixel(4.0f, nkentseu::math::NkVec2f{200.0f, 100.0f}, viewport);
        v.PanByPixels(-310.0f, 145.0f);
        v.Reset();
        return Proche(v.center.x, 0.0) && Proche(v.center.y, 0.0) && Proche(v.zoom, 1.0);
}

struct Test {
        const char *nom;
        bool (*fonction)();
};

void Lancer(const Test &test) {
        const bool ok = test.fonction();
        std::printf("TEST %s : %s\n", test.nom, ok ? "OK" : "ECHEC");
}

} // namespace

int main() {
        const Test tests[5] = {{"aller_retour", aller_retour},
                                                     {"point_fixe", point_fixe},
                                                     {"bornes", bornes},
                                                     {"grille", grille},
                                                     {"origine", origine}};
        for (int i = 0; i < 5; ++i)
                Lancer(tests[i]);
        return 0;
}
