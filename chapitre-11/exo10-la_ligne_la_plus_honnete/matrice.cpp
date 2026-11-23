#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxLignes = 64;
const int kMaxNom = 32;
const int kMaxMot = 24;
const int kTampon = 1 << 16;

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

void Ecrire(const char *txt) {
        fputs(txt, stdout);
}

void EcrireEntier(long long v) {
        char chiffres[24];
        int k = 0;
        if (v == 0)
                chiffres[k++] = '0';
        while (v > 0) {
                chiffres[k++] = (char)('0' + (int)(v % 10));
                v /= 10;
        }
        char ordre[24];
        int m = 0;
        while (k > 0)
                ordre[m++] = chiffres[--k];
        ordre[m] = 0;
        Ecrire(ordre);
}

struct Ligne {
        char nom[kMaxNom];
        long long valeur;
        long long cout;
        char etiquette[kMaxMot];
};

Ligne gLignes[kMaxLignes];
int gNbLignes = 0;

const char *Decision(long long valeur, long long cout) {
        if (cout >= 1000 && valeur <= 400)
                return "ON_NE_LE_FAIT_PAS";
        if (cout > valeur)
                return "DECOUPER";
        if (cout * 3 <= valeur)
                return "EN_PASSANT";
        return "MAINTENANT";
}

void Traiter() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        const int n = atoi(mot);

        for (int i = 0; i < n && gNbLignes < kMaxLignes; ++i) {
                Ligne &l = gLignes[gNbLignes];
                LireMot(l.nom, sizeof(l.nom));
                char valeur[32];
                char cout[32];
                LireMot(valeur, sizeof(valeur));
                LireMot(cout, sizeof(cout));
                l.valeur = strtoll(valeur, nullptr, 10);
                l.cout = strtoll(cout, nullptr, 10);
                LireMot(l.etiquette, sizeof(l.etiquette));
                ++gNbLignes;
        }

        long long refusees = 0;
        long long supposees = 0;

        for (int i = 0; i < gNbLignes; ++i) {
                const Ligne &l = gLignes[i];
                const char *d = Decision(l.valeur, l.cout);

                Ecrire("CASE ");
                Ecrire(l.nom);
                Ecrire(" VALEUR ");
                EcrireEntier(l.valeur);
                Ecrire(" COUT ");
                EcrireEntier(l.cout);
                Ecrire(" ");
                Ecrire(l.etiquette);
                Ecrire(" ");
                Ecrire(d);
                Ecrire("\n");

                if (strcmp(d, "ON_NE_LE_FAIT_PAS") == 0)
                        ++refusees;
                if (strcmp(l.etiquette, "MESUREE") != 0)
                        ++supposees;
        }

        Ecrire("LIGNES ");
        EcrireEntier(gNbLignes);
        Ecrire(" REFUSEES ");
        EcrireEntier(refusees);
        Ecrire(" SUPPOSEES ");
        EcrireEntier(supposees);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
