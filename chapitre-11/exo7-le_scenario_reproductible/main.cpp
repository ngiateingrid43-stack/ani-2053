#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxPas = 100000;
const int kMaxVariable = 100000;
const int kMaxNom = 48;
const int kMaxElement = 32;
const int kMaxElements = 8;
const int kTampon = 1 << 20;

char gTampon[kTampon];
int gAt = 0;
int gLen = 0;

void Remplir() {
        if (gAt < gLen)
                return;
        gLen = (int)fread(gTampon, 1, kTampon, stdin);
        gAt = 0;
}

char LireChar() {
        Remplir();
        if (gAt >= gLen)
                return 0;
        return gTampon[gAt++];
}

void LireMot(char *dst, int cap) {
        int n = 0;
        char c = LireChar();
        while (c != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
                if (n < cap - 1)
                        dst[n++] = c;
                c = LireChar();
        }
        dst[n] = 0;
}

int LireInt() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        return atoi(mot);
}

void Ecrire(const char *txt) {
        fputs(txt, stdout);
}

void EcrireEntier(long long v) {
        char txt[24];
        int n = 0;
        if (v <= 0) {
                txt[n++] = '0';
        } else {
                char tmp[24];
                int m = 0;
                while (v > 0) {
                        tmp[m++] = (char)('0' + (int)(v % 10));
                        v /= 10;
                }
                while (m > 0)
                        txt[n++] = tmp[--m];
        }
        txt[n] = 0;
        Ecrire(txt);
}

struct Pas {
        int tour;
        char geste[kMaxNom];
        int nbElements;
        char elements[kMaxElements][kMaxElement];
        bool capture;
        bool garde;
};

Pas gPas[kMaxPas];
int gNbPas = 0;

bool EstUnGeste(const char *mot) {
        return strcmp(mot, "CLICK") == 0 || strcmp(mot, "DROP") == 0 || strcmp(mot, "DRAG") == 0 ||
               strcmp(mot, "KEY") == 0 || strcmp(mot, "WAIT") == 0;
}

void Trier() {
        for (int i = 1; i < gNbPas; ++i) {
                const Pas cle = gPas[i];
                int j = i - 1;
                while (j >= 0 && gPas[j].tour > cle.tour) {
                        gPas[j + 1] = gPas[j];
                        --j;
                }
                gPas[j + 1] = cle;
        }
}

struct Sortie {
        int tour;
        int rang;
        char texte[kMaxNom + kMaxElement * kMaxElements];
};

Sortie gSorties[kMaxPas];
int gNbSorties = 0;

void Ajouter(int tour, int rang, const char *texte) {
        if (gNbSorties >= kMaxPas)
                return;
        Sortie &s = gSorties[gNbSorties++];
        s.tour = tour;
        s.rang = rang;
        strncpy(s.texte, texte, sizeof(s.texte) - 1);
        s.texte[sizeof(s.texte) - 1] = 0;
}

void TrierSorties() {
        for (int i = 1; i < gNbSorties; ++i) {
                const Sortie cle = gSorties[i];
                int j = i - 1;
                while (j >= 0 && (gSorties[j].tour > cle.tour ||
                                  (gSorties[j].tour == cle.tour && gSorties[j].rang > cle.rang))) {
                        gSorties[j + 1] = gSorties[j];
                        --j;
                }
                gSorties[j + 1] = cle;
        }
}

void Traiter() {
        int n = LireInt();
        if (n < 0)
                n = 0;
        if (n > kMaxVariable)
                n = kMaxVariable;

        for (int i = 0; i < n && gNbPas < kMaxPas; ++i) {
                const int tour = LireInt();
                char geste[kMaxNom];
                LireMot(geste, sizeof(geste));
                const int nb = LireInt();
                Pas &p = gPas[gNbPas];
                p.tour = tour;
                p.nbElements = 0;
                p.capture = (strcmp(geste, "SHOT") == 0);
                p.garde = p.capture || EstUnGeste(geste);
                strncpy(p.geste, geste, sizeof(p.geste) - 1);
                p.geste[sizeof(p.geste) - 1] = 0;
                for (int k = 0; k < nb && k < kMaxElements; ++k) {
                        LireMot(p.elements[k], kMaxElement);
                        ++p.nbElements;
                }
                for (int k = kMaxElements; k < nb; ++k) {
                        char perdu[kMaxElement];
                        LireMot(perdu, sizeof(perdu));
                }
                ++gNbPas;
        }

        Trier();

        int dernierTour = 0;
        for (int i = 0; i < gNbPas; ++i)
                if (gPas[i].tour > dernierTour)
                        dernierTour = gPas[i].tour;

        long long ignores = 0;
        long long jamais = 0;
        long long arret = 0;
        bool arrete = false;

        for (int i = 0; i < gNbPas; ++i) {
                const Pas &p = gPas[i];

                if (p.tour <= 0 || p.tour > dernierTour) {
                        ++ignores;
                        continue;
                }
                if (i > 0 && gPas[i - 1].tour == p.tour && gPas[i - 1].capture && p.capture) {
                        ++ignores;
                        continue;
                }
                if (!p.garde) {
                        ++ignores;
                        continue;
                }

                if (p.capture) {
                        const int tourCapture = p.tour + 1;
                        if (tourCapture > dernierTour) {
                                ++jamais;
                                if (!arrete) {
                                        arret = tourCapture;
                                        arrete = true;
                                }
                                continue;
                        }
                        char texte[kMaxNom];
                        strcpy(texte, "SHOT");
                        Ajouter(tourCapture, 0, texte);
                        continue;
                }

                char texte[kMaxNom + kMaxElement * kMaxElements];
                strcpy(texte, p.geste);
                for (int k = 0; k < p.nbElements; ++k) {
                        strcat(texte, " ");
                        strncat(texte, p.elements[k], kMaxElement - 1);
                }
                Ajouter(p.tour, 1, texte);
        }

        TrierSorties();

        for (int i = 0; i < gNbSorties; ++i) {
                Ecrire("TOUR ");
                EcrireEntier(gSorties[i].tour);
                Ecrire(" ");
                Ecrire(gSorties[i].texte);
                Ecrire("\n");
        }

        Ecrire("ARRET ");
        EcrireEntier(arret);
        Ecrire("\nJAMAIS ");
        EcrireEntier(jamais);
        Ecrire("\nIGNORES ");
        EcrireEntier(ignores);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
