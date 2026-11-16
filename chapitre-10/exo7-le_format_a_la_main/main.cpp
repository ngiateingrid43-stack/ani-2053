#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

namespace {

unsigned char *gOctets = nullptr;
long long gTaille = 0;
long long gAt = 0;
bool gTronque = false;

char gBuffer[1 << 20];
int gBufAt = 0;
int gBufLen = 0;

void Remplir() {
        if (gBufAt < gBufLen)
                return;
        gBufLen = (int)fread(gBuffer, 1, sizeof(gBuffer), stdin);
        gBufAt = 0;
}

char LireChar() {
        Remplir();
        if (gBufAt >= gBufLen)
                return 0;
        return gBuffer[gBufAt++];
}

int LireMot(char *dst, int cap) {
        int n = 0;
        char c = LireChar();
        while (c != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
                if (n < cap - 1)
                        dst[n++] = c;
                c = LireChar();
        }
        dst[n] = 0;
        return n;
}

int ValeurHexa(char c) {
        if (c >= '0' && c <= '9')
                return c - '0';
        if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;
        if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
        return -1;
}

bool Reste(long long combien) {
        if (gTronque)
                return false;
        if (combien < 0 || gAt + combien > gTaille) {
                gTronque = true;
                return false;
        }
        return true;
}

unsigned int LireU8() {
        if (!Reste(1))
                return 0;
        return gOctets[gAt++];
}

unsigned long LireU32() {
        if (!Reste(4))
                return 0;
        unsigned long v = 0;
        for (int i = 0; i < 4; ++i)
                v |= ((unsigned long)gOctets[gAt + i]) << (8 * i);
        gAt += 4;
        return v;
}

float LireF32() {
        if (!Reste(4))
                return 0.0f;
        unsigned long brut = 0;
        for (int i = 0; i < 4; ++i)
                brut |= ((unsigned long)gOctets[gAt + i]) << (8 * i);
        gAt += 4;
        float v = 0.0f;
        memcpy(&v, &brut, 4);
        return v;
}

void Sauter(long long combien) {
        if (!Reste(combien))
                return;
        gAt += combien;
}

void EcrireDeuxChiffres(double v) {
        long long centiemes = (long long)llround(v * 100.0);
        if (centiemes == 0) {
                fputs("0.00", stdout);
                return;
        }
        if (centiemes < 0) {
                fputc('-', stdout);
                centiemes = -centiemes;
        }
        printf("%lld.", centiemes / 100);
        const long long reste = centiemes % 100;
        if (reste < 10)
                fputc('0', stdout);
        printf("%lld", reste);
}

void Parcourir(bool imprimer) {
        gAt = 0;
        const unsigned int m0 = LireU8(), m1 = LireU8(), m2 = LireU8(), m3 = LireU8();
        if (gTronque || m0 != 'N' || m1 != 'K' || m2 != 'R' || m3 != 'F')
                return;
        const unsigned long version = LireU32();
        if (gTronque || version != 1)
                return;

        const float cx = LireF32();
        const float cy = LireF32();
        const float zoom = LireF32();
        const unsigned int theme = LireU8();
        const unsigned int grille = LireU8();
        LireU8();
        LireU8();
        const unsigned long nbImages = LireU32();
        if (gTronque || nbImages > 1000000ul)
                return;

        if (imprimer) {
                fputs("VUE ", stdout);
                EcrireDeuxChiffres(cx);
                fputc(' ', stdout);
                EcrireDeuxChiffres(cy);
                fputc(' ', stdout);
                EcrireDeuxChiffres(zoom);
                fputc('\n', stdout);
                printf("%s\n", theme != 0 ? "THEME SOMBRE" : "THEME CLAIR");
                printf("%s\n", grille == 0 ? "GRILLE AFFICHEE" : "GRILLE CACHEE");
                printf("IMAGES %lu\n", nbImages);
        }

        for (unsigned long i = 0; i < nbImages && !gTronque; ++i) {
                const float x = LireF32();
                const float y = LireF32();
                const float echelle = LireF32();
                const float rotation = LireF32();
                const float opacite = LireF32();
                const unsigned int mx = LireU8() != 0 ? 1u : 0u;
                const unsigned int my = LireU8() != 0 ? 1u : 0u;
                LireU8();
                LireU8();
                const unsigned long largeur = LireU32();
                const unsigned long hauteur = LireU32();
                const unsigned long taille = LireU32();
                if (gTronque)
                        return;
                if (imprimer) {
                        printf("IMAGE %lu ", i + 1);
                        EcrireDeuxChiffres(x);
                        fputc(' ', stdout);
                        EcrireDeuxChiffres(y);
                        fputc(' ', stdout);
                        EcrireDeuxChiffres(echelle);
                        fputc(' ', stdout);
                        EcrireDeuxChiffres(rotation);
                        fputc(' ', stdout);
                        EcrireDeuxChiffres(opacite);
                        printf(" %u %u %lu %lu %lu\n", mx, my, largeur, hauteur, taille);
                }
                Sauter((long long)taille);
                if (gTronque)
                        return;
        }

        const unsigned long nbTraits = LireU32();
        if (gTronque || nbTraits > 1000000ul)
                return;
        if (imprimer)
                printf("TRAITS %lu\n", nbTraits);

        for (unsigned long s = 0; s < nbTraits && !gTronque; ++s) {
                const unsigned int r = LireU8();
                const unsigned int g = LireU8();
                const unsigned int b = LireU8();
                const unsigned int a = LireU8();
                const float epaisseur = LireF32();
                const unsigned long nbPoints = LireU32();
                if (gTronque || nbPoints > 10000000ul)
                        return;
                if (imprimer) {
                        printf("TRAIT %lu %u %u %u %u ", s + 1, r, g, b, a);
                        EcrireDeuxChiffres(epaisseur);
                        printf(" %lu\n", nbPoints);
                }
                for (unsigned long p = 0; p < nbPoints && !gTronque; ++p) {
                        const float px = LireF32();
                        const float py = LireF32();
                        if (gTronque)
                                return;
                        if (imprimer) {
                                fputs("POINT ", stdout);
                                EcrireDeuxChiffres(px);
                                fputc(' ', stdout);
                                EcrireDeuxChiffres(py);
                                fputc('\n', stdout);
                        }
                }
        }
}

void Traiter() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        const long long t = (long long)strtoll(mot, nullptr, 10);
        if (t < 0) {
                printf("TRONQUE\n");
                return;
        }
        gTaille = t;
        gOctets = (unsigned char *)malloc((size_t)(t > 0 ? t : 1));
        if (gOctets == nullptr) {
                printf("TRONQUE\n");
                return;
        }
        long long lus = 0;
        char octet[8];
        while (lus < t) {
                if (LireMot(octet, sizeof(octet)) == 0)
                        break;
                const int haut = ValeurHexa(octet[0]);
                const int bas = octet[1] == 0 ? -1 : ValeurHexa(octet[1]);
                if (haut < 0 || bas < 0)
                        break;
                gOctets[lus++] = (unsigned char)(haut * 16 + bas);
        }
        if (lus < t)
                gTaille = lus;

        if (gTaille < 4) {
                printf("TRONQUE\n");
                free(gOctets);
                return;
        }
        if (!(gOctets[0] == 'N' && gOctets[1] == 'K' && gOctets[2] == 'R' && gOctets[3] == 'F')) {
                printf("PAS_UNE_PLANCHE\n");
                free(gOctets);
                return;
        }
        if (gTaille < 8) {
                printf("TRONQUE\n");
                free(gOctets);
                return;
        }
        unsigned long version = 0;
        for (int i = 0; i < 4; ++i)
                version |= ((unsigned long)gOctets[4 + i]) << (8 * i);
        if (version != 1) {
                printf("VERSION_INCONNUE %lu\n", version);
                free(gOctets);
                return;
        }

        gAt = 0;
        gTronque = false;
        Parcourir(false);
        if (gTronque) {
                printf("TRONQUE\n");
                free(gOctets);
                return;
        }
        gAt = 0;
        gTronque = false;
        Parcourir(true);
        free(gOctets);
}

} // namespace

int main() {
        Traiter();
        return 0;
}
