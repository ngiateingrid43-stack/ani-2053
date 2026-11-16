#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxItems = 100000;
const int kMaxClicks = 200000;
const int kBufferSize = 1 << 20;

char gBuffer[kBufferSize];
int gAt = 0;
int gLen = 0;

void Remplir() {
        if (gAt < gLen)
                return;
        gLen = (int)fread(gBuffer, 1, kBufferSize, stdin);
        gAt = 0;
}

char LireChar() {
        Remplir();
        if (gAt >= gLen)
                return 0;
        return gBuffer[gAt++];
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

double LireNombre() {
        char mot[64];
        LireMot(mot, sizeof(mot));
        return strtod(mot, nullptr);
}

int LireInt() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        return atoi(mot);
}


struct Image {
        char nom[64];
        double x;
        double y;
        double demiLargeur;
        double demiHauteur;
};

Image gImages[kMaxItems];

bool Touche(const Image &img, double px, double py) {
        const double dx = px - img.x;
        const double dy = py - img.y;
        return dx >= -img.demiLargeur && dx <= img.demiLargeur && dy >= -img.demiHauteur && dy <= img.demiHauteur;
}

const char *ChercherARebours(int n, double px, double py) {
        for (int i = n - 1; i >= 0; --i)
                if (Touche(gImages[i], px, py))
                        return gImages[i].nom;
        return "AUCUN";
}

const char *ChercherALEndroit(int n, double px, double py) {
        for (int i = 0; i < n; ++i)
                if (Touche(gImages[i], px, py))
                        return gImages[i].nom;
        return "AUCUN";
}

void Traiter() {
        int n = LireInt();
        if (n < 0)
                n = 0;
        if (n > kMaxItems)
                n = kMaxItems;
        for (int i = 0; i < n; ++i) {
                LireMot(gImages[i].nom, sizeof(gImages[i].nom));
                gImages[i].x = LireNombre();
                gImages[i].y = LireNombre();
                const double largeur = LireNombre();
                const double hauteur = LireNombre();
                const double echelle = LireNombre();
                gImages[i].demiLargeur = largeur * echelle * 0.5;
                gImages[i].demiHauteur = hauteur * echelle * 0.5;
        }

        const int q = LireInt();
        int clics = q < 0 ? 0 : q;
        if (clics > kMaxClicks)
                clics = kMaxClicks;
        int erreurs = 0;
        for (int k = 0; k < clics; ++k) {
                const double px = LireNombre();
                const double py = LireNombre();
                const char *a = ChercherARebours(n, px, py);
                const char *b = ChercherALEndroit(n, px, py);
                if (strcmp(a, b) != 0)
                        ++erreurs;
                char ligne[256];
                snprintf(ligne, sizeof(ligne), "CLIC %d %s %s\n", k + 1, a, b);
                fputs(ligne, stdout);
        }
        char ligne[64];
        snprintf(ligne, sizeof(ligne), "ERREURS %d\n", erreurs);
        fputs(ligne, stdout);
}

} // namespace

int main() {
        Traiter();
        return 0;
}
