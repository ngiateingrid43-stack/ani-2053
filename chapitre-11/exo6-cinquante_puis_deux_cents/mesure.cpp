#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "NkRef/NkRefFile.h"

using namespace nkentseu;
using namespace nkref;

namespace {

// 32 octets d'en-tete (signature, version, vue, theme, grille, nbItems), puis
// 36 octets par image et les octets de l'image. Rien d'autre.
const int kOctetsParImage = 1000;

uint32 Crc32(const uint8 *p, usize n) {
        static uint32 table[256];
        static bool prete = false;
        if (!prete) {
                for (uint32 i = 0; i < 256; ++i) {
                        uint32 c = i;
                        for (int k = 0; k < 8; ++k)
                                c = (c & 1u) ? (0xedb88320u ^ (c >> 1)) : (c >> 1);
                        table[i] = c;
                }
                prete = true;
        }
        uint32 c = 0xffffffffu;
        for (usize i = 0; i < n; ++i)
                c = table[(c ^ p[i]) & 0xffu] ^ (c >> 8);
        return c ^ 0xffffffffu;
}

void PutU32BE(NkVector<uint8> &out, uint32 v) {
        out.PushBack((uint8)(v >> 24));
        out.PushBack((uint8)(v >> 16));
        out.PushBack((uint8)(v >> 8));
        out.PushBack((uint8)v);
}

void PutChunk(NkVector<uint8> &out, const char *type, const uint8 *data, usize n) {
        PutU32BE(out, (uint32)n);
        const usize debut = out.Size();
        for (int i = 0; i < 4; ++i)
                out.PushBack((uint8)type[i]);
        for (usize i = 0; i < n; ++i)
                out.PushBack(data[i]);
        PutU32BE(out, Crc32(out.Data() + debut, n + 4));
}

// Un PNG valide de 4 x 4 pixels, complete a exactement 1000 octets par un bloc
// de texte libre : c'est la taille des images que le protocole de mesure fixe.
void FaireImage(NkVector<uint8> &sortie, uint8 teinte) {
        sortie.Clear();
        static const uint8 signature[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};
        for (int i = 0; i < 8; ++i)
                sortie.PushBack(signature[i]);

        uint8 ihdr[13];
        ihdr[0] = 0;
        ihdr[1] = 0;
        ihdr[2] = 0;
        ihdr[3] = 4;
        ihdr[4] = 0;
        ihdr[5] = 0;
        ihdr[6] = 0;
        ihdr[7] = 4;
        ihdr[8] = 8;
        ihdr[9] = 6;
        ihdr[10] = 0;
        ihdr[11] = 0;
        ihdr[12] = 0;
        PutChunk(sortie, "IHDR", ihdr, 13);

        NkVector<uint8> brut;
        for (int y = 0; y < 4; ++y) {
                brut.PushBack(0);
                for (int x = 0; x < 4; ++x) {
                        brut.PushBack((uint8)(teinte + (uint8)(x * 20)));
                        brut.PushBack((uint8)(teinte + (uint8)(y * 20)));
                        brut.PushBack((uint8)(teinte + 60));
                        brut.PushBack(255);
                }
        }

        NkVector<uint8> z;
        z.PushBack(0x78);
        z.PushBack(0x01);
        const usize n = brut.Size();
        z.PushBack(0x01);
        z.PushBack((uint8)(n & 0xffu));
        z.PushBack((uint8)((n >> 8) & 0xffu));
        z.PushBack((uint8)(~n & 0xffu));
        z.PushBack((uint8)((~n >> 8) & 0xffu));
        for (usize i = 0; i < n; ++i)
                z.PushBack(brut[i]);

        uint32 a = 1;
        uint32 b = 0;
        for (usize i = 0; i < n; ++i) {
                a = (a + brut[i]) % 65521u;
                b = (b + a) % 65521u;
        }
        PutU32BE(z, (b << 16) | a);
        PutChunk(sortie, "IDAT", z.Data(), z.Size());

        const usize fixe = sortie.Size() + 12 + 12 + 8;
        const usize reste = (usize)kOctetsParImage > fixe ? (usize)kOctetsParImage - fixe : 0;
        NkVector<uint8> texte;
        const char *mot = "Comment";
        for (int i = 0; mot[i] != 0; ++i)
                texte.PushBack((uint8)mot[i]);
        texte.PushBack(0);
        for (usize i = 0; i < reste; ++i)
                texte.PushBack((uint8)('a' + (uint8)((i + teinte) % 26u)));
        PutChunk(sortie, "tEXt", texte.Data(), texte.Size());
        PutChunk(sortie, "IEND", nullptr, 0);
}

const uint32 kLargeurs[] = {4, 4, 4};
const uint32 kHauteurs[] = {4, 4, 4};

NkVector<uint8> gImages[3];

void Construire(int n, NkRefBoard &planche, NkVector<NkRefFileImage> &images) {
        for (int i = 0; i < n; ++i) {
                NkRefItem item;
                item.id = (uint32)(i + 1);
                item.pos.x = (float32)(i % 10) * 40.0f;
                item.pos.y = (float32)(i / 10) * 40.0f;
                item.scale = 1.0f;
                item.rotationDeg = 0.0f;
                item.opacity = 1.0f;
                item.mirrorX = false;
                item.mirrorY = false;
                item.texW = kLargeurs[i % 3];
                item.texH = kHauteurs[i % 3];
                planche.items.PushBack(item);

                NkRefFileImage img;
                img.data = gImages[i % 3].Data();
                img.size = gImages[i % 3].Size();
                images.PushBack(img);
        }
}

unsigned long Mesurer(int n) {
        NkRefBoard planche;
        NkVector<NkRefFileImage> images;
        Construire(n, planche, images);

        NkRefView vue;
        vue.center.x = 0.0f;
        vue.center.y = 0.0f;
        vue.zoom = 1.0f;

        NkVector<uint8> fichier;
        NkRefSerialize(fichier, planche, images.Data(), vue, false, true);

        NkVector<uint8> temoin;
        NkRefSerialize(temoin, planche, images.Data(), vue, false, true);
        if (fichier.Size() != temoin.Size()) {
                std::fprintf(stderr, "mesure : deux passages ne donnent pas la meme taille\n");
                std::exit(2);
        }
        bool identiques = true;
        for (usize i = 0; i < fichier.Size(); ++i)
                if (fichier[i] != temoin[i])
                        identiques = false;
        if (!identiques) {
                std::fprintf(stderr, "mesure : deux passages ne donnent pas les memes octets\n");
                std::exit(3);
        }
        return (unsigned long)fichier.Size();
}

} // namespace

int main() {
        for (int i = 0; i < 3; ++i) {
                FaireImage(gImages[i], (uint8)(i * 70));
                if (gImages[i].Size() != (usize)kOctetsParImage) {
                        std::fprintf(stderr, "mesure : image %d de %lu octets au lieu de %d\n", i,
                                     (unsigned long)gImages[i].Size(), kOctetsParImage);
                        return 1;
                }
        }

        std::printf("Cinquante : %lu\n", Mesurer(50));
        std::printf("Deux cents : %lu\n", Mesurer(200));
        return 0;
}
