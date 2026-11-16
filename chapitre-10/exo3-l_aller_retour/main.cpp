#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

namespace {

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

struct Camera {
        double cx = 0.0;
        double cy = 0.0;
        double zoom = 1.0;
};

Camera gCam;

void Ecrire(const char *txt) {
        fputs(txt, stdout);
}

void EcrireEntier(long long v) {
        char txt[32];
        int n = 0;
        if (v == 0) {
                txt[n++] = '0';
        } else {
                char tmp[32];
                int m = 0;
                long long x = v;
                while (x > 0) {
                        tmp[m++] = (char)('0' + (int)(x % 10));
                        x /= 10;
                }
                while (m > 0)
                        txt[n++] = tmp[--m];
        }
        txt[n] = 0;
        fputs(txt, stdout);
}

double ArrondiAuZero(double v) {
        return v == 0.0 ? 0.0 : v;
}

void EcrireDeuxChiffres(double v) {
        long long centiemes = (long long)llround(v * 100.0);
        if (centiemes == 0) {
                Ecrire("0.00");
                return;
        }
        if (centiemes < 0) {
                Ecrire("-");
                centiemes = -centiemes;
        }
        EcrireEntier(centiemes / 100);
        Ecrire(".");
        const long long reste = centiemes % 100;
        if (reste < 10)
                Ecrire("0");
        EcrireEntier(reste);
}

void EcrireSixChiffres(double v) {
        long long millioniemes = (long long)llround(v * 1000000.0);
        if (millioniemes == 0) {
                Ecrire("0.000000");
                return;
        }
        if (millioniemes < 0) {
                Ecrire("-");
                millioniemes = -millioniemes;
        }
        EcrireEntier(millioniemes / 1000000);
        Ecrire(".");
        long long reste = millioniemes % 1000000;
        char chiffres[8];
        for (int i = 5; i >= 0; --i) {
                chiffres[i] = (char)('0' + (int)(reste % 10));
                reste /= 10;
        }
        for (int i = 0; i < 6; ++i)
                fputc(chiffres[i], stdout);
}

void Traiter() {
        double largeur = LireNombre();
        double hauteur = LireNombre();
        const double demiLargeur = largeur * 0.5;
        const double demiHauteur = hauteur * 0.5;
        char commande[32];
        while (LireMot(commande, sizeof(commande)) > 0) {
                if (strcmp(commande, "VERS_MONDE") == 0) {
                        const double px = LireNombre();
                        const double py = LireNombre();
                        const double x = gCam.cx + (px - demiLargeur) / gCam.zoom;
                        const double y = gCam.cy + (py - demiHauteur) / gCam.zoom;
                        Ecrire("MONDE ");
                        EcrireDeuxChiffres(ArrondiAuZero(x));
                        Ecrire(" ");
                        EcrireDeuxChiffres(ArrondiAuZero(y));
                        Ecrire("\n");
                } else if (strcmp(commande, "VERS_PIXEL") == 0) {
                        const double mx = LireNombre();
                        const double my = LireNombre();
                        const double x = (mx - gCam.cx) * gCam.zoom + demiLargeur;
                        const double y = (my - gCam.cy) * gCam.zoom + demiHauteur;
                        Ecrire("PIXEL ");
                        EcrireDeuxChiffres(ArrondiAuZero(x));
                        Ecrire(" ");
                        EcrireDeuxChiffres(ArrondiAuZero(y));
                        Ecrire("\n");
                } else if (strcmp(commande, "DEPLACE") == 0) {
                        const double dx = LireNombre();
                        const double dy = LireNombre();
                        gCam.cx -= dx / gCam.zoom;
                        gCam.cy -= dy / gCam.zoom;
                } else if (strcmp(commande, "ZOOM") == 0) {
                        const double facteur = LireNombre();
                        const double px = LireNombre();
                        const double py = LireNombre();
                        const double ax = gCam.cx + (px - demiLargeur) / gCam.zoom;
                        const double ay = gCam.cy + (py - demiHauteur) / gCam.zoom;
                        double z = gCam.zoom * facteur;
                        if (z < 0.02)
                                z = 0.02;
                        if (z > 64.0)
                                z = 64.0;
                        gCam.zoom = z;
                        gCam.cx = ax - (px - demiLargeur) / gCam.zoom;
                        gCam.cy = ay - (py - demiHauteur) / gCam.zoom;
                } else if (strcmp(commande, "GRILLE") == 0) {
                        const double m = LireNombre();
                        double s = 1.0;
                        while (s * gCam.zoom < m)
                                s *= 2.0;
                        while (s * 0.5 * gCam.zoom >= m)
                                s *= 0.5;
                        Ecrire("GRILLE ");
                        EcrireSixChiffres(ArrondiAuZero(s));
                        Ecrire("\n");
                } else if (strcmp(commande, "ORIGINE") == 0) {
                        gCam.cx = 0.0;
                        gCam.cy = 0.0;
                        gCam.zoom = 1.0;
                }
        }
        Ecrire("CENTRE ");
        EcrireDeuxChiffres(ArrondiAuZero(gCam.cx));
        Ecrire(" ");
        EcrireDeuxChiffres(ArrondiAuZero(gCam.cy));
        Ecrire("\nZOOM ");
        EcrireDeuxChiffres(ArrondiAuZero(gCam.zoom));
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
