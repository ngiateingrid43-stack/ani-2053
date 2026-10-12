// Exercice 4 - L'atlas, vu
// Construit un atlas de police avec NKFont, enregistre sa texture en PNG (atlas.png),
// compte les glyphes, estime la place perdue et ecrit compte.txt, a deux tailles (18 puis 48).

#include "NKWindow/NKMain.h"
#include "NKLogger/NkLog.h"
#include "NKFont/NkFont.h"
#include "NKImage/Core/NkImage.h"

#include <cstdio>

using namespace nkentseu;

NKENTSEU_DEFINE_APP_DATA(([]() {
    NkAppData d{};
    d.appName = "Atlas";
    d.appVersion = "1.0.0";
    return d;
})());

static bool FichierExiste(const char *chemin) {
    FILE *f = std::fopen(chemin, "rb");
    if (!f) return false;
    std::fclose(f);
    return true;
}

// Construit l'atlas a la taille donnee. Si sauverPng : ecrit atlas.png.
static bool ConstruireAtlas(const char *police, float taille, bool sauverPng, FILE *compte) {
    NkFontAtlas atlas; // l'atlas possede la texture et les glyphes : il doit vivre jusqu'a la fin

    // 1. Ajouter la police, PUIS construire (Build rasterise tout d'un coup).
    NkFont *face = atlas.AddFontFromFile(police, taille);
    if (!face) {
        logger.Errorf("[atlas] police introuvable ou illisible : %s", police);
        return false;
    }
    if (!atlas.Build()) {
        logger.Error("[atlas] Build() a echoue");
        return false;
    }

    // 2. Recuperer la texture RGBA32.
    nkft_uint8 *pixels = nullptr;
    nkft_int32 largeur = 0, hauteur = 0;
    atlas.GetTexDataAsRGBA32(&pixels, &largeur, &hauteur);
    if (!pixels || largeur <= 0 || hauteur <= 0) {
        logger.Error("[atlas] aucune texture produite");
        return false;
    }

    // 3. Place perdue : surface de la texture moins la somme des rectangles des glyphes
    //    (rectangle UV de chaque glyphe visible, converti en pixels). Le reste, ce sont
    //    les marges entre glyphes et les etageres inoccupees.
    const long long surface = static_cast<long long>(largeur) * static_cast<long long>(hauteur);
    double occupe = 0.0;
    for (nkft_uint32 i = 0; i < face->glyphCount; ++i) {
        const NkFontGlyph &g = face->glyphs[i];
        if (!g.visible) continue;
        const double gw = static_cast<double>(g.u1 - g.u0) * static_cast<double>(largeur);
        const double gh = static_cast<double>(g.v1 - g.v0) * static_cast<double>(hauteur);
        occupe += gw * gh;
    }
    const int perdu = static_cast<int>((static_cast<double>(surface) - occupe) * 100.0 / static_cast<double>(surface));

    logger.Infof("[atlas] taille=%d glyphes=%d texture=%dx%d perdu=%d%%", static_cast<int>(taille),
                 static_cast<int>(face->glyphCount), static_cast<int>(largeur), static_cast<int>(hauteur), perdu);

    // 4. Enregistrer en PNG. NkImage est un type valeur : Wrap rend une vue NON-OWNING
    //    (aucune copie de pixels). Le destructeur de la vue ne libere rien : les pixels
    //    appartiennent a l'atlas, qui reste en vie jusqu'a la fin de la fonction.
    //    Plus de nullptr ni de Free() : on teste IsValid().
    if (sauverPng) {
        NkImage img = NkImage::Wrap(reinterpret_cast<uint8 *>(pixels), static_cast<int32>(largeur),
                                    static_cast<int32>(hauteur), NkImagePixelFormat::NK_RGBA32);
        if (!img.IsValid()) {
            logger.Error("[atlas] Wrap a echoue");
            return false;
        }
        if (!img.SavePNG("atlas.png")) {
            logger.Error("[atlas] SavePNG a echoue");
            return false;
        }
    }

    std::fprintf(compte, "taille : %d, glyphes : %d, texture : %d %d, perdu : %d\n", static_cast<int>(taille),
                 static_cast<int>(face->glyphCount), static_cast<int>(largeur), static_cast<int>(hauteur), perdu);
    return true;
}

int nkmain(const NkEntryState &state) {
    const char *police = "C:/Windows/Fonts/ALGER.TTF";
    if (state.args.Size() > 1) police = state.args[1].CStr();
    if (!FichierExiste(police)) {
        logger.Errorf("[atlas] fichier de police absent : %s (lancer depuis la racine du depot)", police);
        return -1;
    }

    FILE *compte = std::fopen("compte.txt", "w");
    if (!compte) {
        logger.Error("[atlas] impossible d'ecrire compte.txt");
        return -2;
    }
    bool ok = ConstruireAtlas(police, 18.f, true, compte);   // avec atlas.png
    ok = ConstruireAtlas(police, 48.f, false, compte) && ok; // sans rendre la seconde image
    std::fclose(compte);
    return ok ? 0 : -3;
}