#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxLignes = 64;
const int kMaxNom = 32;
const int kMaxMot = 32;
const int kSeuilCher = 1000;
const int kSeuilFaible = 400;
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

int LireInt() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        return atoi(mot);
}

int LireEtiquette() {
        char mot[kMaxMot];
        LireMot(mot, sizeof(mot));
        if (strcmp(mot, "MESUREE") == 0)
                return 3;
        if (strcmp(mot, "OBSERVEE") == 0)
                return 2;
        if (strcmp(mot, "SUPPOSEE") == 0)
                return 1;
        return 0;
}

void Ecrire(const char *txt) {
        fputs(txt, stdout);
}

void EcrireEntier(int v) {
        char txt[24];
        int n = 0;
        if (v <= 0) {
                txt[n++] = '0';
        } else {
                char tmp[24];
                int m = 0;
                while (v > 0) {
                        tmp[m++] = (char)('0' + (v % 10));
                        v /= 10;
                }
                while (m > 0)
                        txt[n++] = tmp[--m];
        }
        txt[n] = 0;
        Ecrire(txt);
}

struct Ligne {
        char nom[kMaxNom];
        int valeur;
        int cout;
        int etiquette;
        const char *verdict;
};

Ligne gLignes[kMaxLignes];
int gNbLignes = 0;

void Traiter() {
        const int n = LireInt();
        for (int i = 0; i < n && gNbLignes < kMaxLignes; ++i) {
                Ligne &l = gLignes[gNbLignes];
                LireMot(l.nom, sizeof(l.nom));
                l.valeur = LireInt();
                l.cout = LireInt();
                l.etiquette = LireEtiquette();
                if (l.cout >= kSeuilCher && l.valeur <= kSeuilFaible)
                        l.verdict = "ON_NE_LE_FAIT_PAS";
                else if (l.cout > l.valeur)
                        l.verdict = "DECOUPER";
                else if (l.cout * 3 <= l.valeur)
                        l.verdict = "EN_PASSANT";
                else
                        l.verdict = "MAINTENANT";
                ++gNbLignes;
        }

        int refuses = 0;
        int supposees = 0;
        int retenues = 0;

        for (int i = 0; i < gNbLignes; ++i) {
                const Ligne &l = gLignes[i];
                Ecrire("CASE ");
                Ecrire(l.nom);
                Ecrire(" VALEUR ");
                EcrireEntier(l.valeur);
                Ecrire(" COUT ");
                EcrireEntier(l.cout);
                Ecrire(" ");
                if (l.etiquette == 3)
                        Ecrire("MESUREE ");
                else if (l.etiquette == 2)
                        Ecrire("OBSERVEE ");
                else if (l.etiquette == 1)
                        Ecrire("SUPPOSEE ");
                else
                        Ecrire("INCONNUE ");
                Ecrire(l.verdict);
                Ecrire("\n");

                if (strcmp(l.verdict, "ON_NE_LE_FAIT_PAS") == 0)
                        ++refuses;
                else
                        ++retenues;
                if (l.etiquette < 3)
                        ++supposees;
        }

        Ecrire("CASES ");
        EcrireEntier(gNbLignes);
        Ecrire("\nRETENUES ");
        EcrireEntier(retenues);
        Ecrire("\nREFUSEES ");
        EcrireEntier(refuses);
        Ecrire("\nSUPPOSEES ");
        EcrireEntier(supposees);
        Ecrire("\n");

        if (refuses >= 1)
                Ecrire("VERDICT HONNETE\n");
        else
                Ecrire("VERDICT COMPLAISANT\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
