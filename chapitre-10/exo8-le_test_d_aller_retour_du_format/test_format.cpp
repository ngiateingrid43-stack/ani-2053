// Compilation, le format cassé d'abord dans le chemin d'inclusion :
//   g++ -std=c++17 -Wall test_format.cpp -o test_format -Icopie -I<racine moteur> <objets du moteur>

#include "NkRef/NkRefFile.h"

#include <cstdio>

namespace {

struct Banc {
        nkref::NkRefBoard planche;
        nkentseu::uint8 sourceA[8];
        nkentseu::uint8 sourceB[5];
        nkref::NkRefFileImage images[2];
        nkref::NkRefView vue;
};

void Construire(Banc &banc) {
        for (int i = 0; i < 8; ++i)
                banc.sourceA[i] = (nkentseu::uint8)(0x89 + i);
        for (int i = 0; i < 5; ++i)
                banc.sourceB[i] = (nkentseu::uint8)(0x10 * (i + 1));
        banc.images[0].data = banc.sourceA;
        banc.images[0].size = 8;
        banc.images[1].data = banc.sourceB;
        banc.images[1].size = 5;

        nkentseu::NkString vide;
        banc.planche.AddItem(nkentseu::math::NkVec2f{-64.0f, 32.0f}, 128, 96, vide);
        banc.planche.items[0].scale = 0.5f;
        banc.planche.items[0].rotationDeg = 90.0f;
        banc.planche.items[0].opacity = 0.75f;
        banc.planche.items[0].mirrorX = true;

        banc.planche.AddItem(nkentseu::math::NkVec2f{200.0f, -12.5f}, 64, 48, vide);
        banc.planche.items[1].scale = 2.0f;
        banc.planche.items[1].rotationDeg = -30.0f;

        banc.planche.BeginStroke(255, 0, 0, 3.0f, nkentseu::math::NkVec2f{0.0f, 0.0f});
        banc.planche.AppendStrokePoint(nkentseu::math::NkVec2f{10.0f, 5.0f}, 0.0f);
        banc.planche.AppendStrokePoint(nkentseu::math::NkVec2f{-3.25f, 8.5f}, 0.0f);

        banc.vue.center = nkentseu::math::NkVec2f{12.5f, -40.0f};
        banc.vue.zoom = 2.0f;
}

void Reconstruire(nkref::NkRefBoard &planche, const nkentseu::NkVector<nkref::NkRefLoadedItem> &relus) {
        for (nkentseu::usize i = 0; i < relus.Size(); ++i) {
                nkentseu::NkString vide;
                const nkref::NkRefItem &lu = relus[i].item;
                planche.AddItem(lu.pos, lu.texW, lu.texH, vide);
                nkref::NkRefItem &cible = planche.items[planche.items.Size() - 1];
                cible.scale = lu.scale;
                cible.rotationDeg = lu.rotationDeg;
                cible.opacity = lu.opacity;
                cible.mirrorX = lu.mirrorX;
                cible.mirrorY = lu.mirrorY;
        }
}

bool Identiques(const nkentseu::NkVector<nkentseu::uint8> &a, const nkentseu::NkVector<nkentseu::uint8> &b) {
        if (a.Size() != b.Size())
                return false;
        for (nkentseu::usize i = 0; i < a.Size(); ++i)
                if (a[i] != b[i])
                        return false;
        return true;
}

} // namespace

int main() {
        Banc banc;
        Construire(banc);

        nkentseu::NkVector<nkentseu::uint8> premiers;
        nkref::NkRefSerialize(premiers, banc.planche, banc.images, banc.vue, true, false);

        nkentseu::NkVector<nkref::NkRefLoadedItem> relus;
        nkentseu::NkVector<nkref::NkRefStroke> traitsRelus;
        nkref::NkRefView vueRelue;
        bool themeRelu = false;
        bool grilleRelue = true;
        const bool lu = nkref::NkRefDeserialize(premiers.Data(), premiers.Size(), relus, traitsRelus, vueRelue,
                                                                                        themeRelu, grilleRelue);

        nkref::NkRefBoard seconde;
        nkref::NkRefFileImage imagesSeconde[2];
        for (int i = 0; i < 2; ++i) {
                imagesSeconde[i].data = i < (int)relus.Size() ? relus[i].src.Data() : nullptr;
                imagesSeconde[i].size = i < (int)relus.Size() ? relus[i].src.Size() : 0;
        }
        Reconstruire(seconde, relus);
        for (nkentseu::usize i = 0; i < traitsRelus.Size(); ++i) {
                const nkref::NkRefStroke &trait = traitsRelus[i];
                seconde.strokes.PushBack(trait);
        }

        nkentseu::NkVector<nkentseu::uint8> seconds;
        nkref::NkRefSerialize(seconds, seconde, imagesSeconde, vueRelue, themeRelu, grilleRelue);

        const bool identiques = Identiques(premiers, seconds);
        std::printf("octets : %llu, relu : %s, identiques : %s\n", (unsigned long long)premiers.Size(),
                                lu ? "OUI" : "NON", identiques ? "OUI" : "NON");
        return 0;
}
