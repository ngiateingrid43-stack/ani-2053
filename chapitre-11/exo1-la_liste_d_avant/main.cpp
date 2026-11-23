#include <cstdio>
#include <cstdlib>

namespace {

const int kMaxLignes = 200000;
const int kMaxCle = 24;
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

int LireDrapeau() {
        char mot[8];
        LireMot(mot, sizeof(mot));
        return mot[0] == '1';
}

void Ecrire(const char *txt) {
        fputs(txt, stdout);
}

void EcrireEntier(long long v) {
        char txt[24];
        int n = 0;
        if (v == 0) {
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

void Traiter() {
        long long n = 0;
        char mot[32];
        LireMot(mot, sizeof(mot));
        n = strtoll(mot, nullptr, 10);
        if (n < 0)
                n = 0;
        if (n > kMaxLignes)
                n = kMaxLignes;

        long long vraies = 0;
        long long fausses = 0;
        long long oubliees = 0;
        long long nouvelles = 0;

        for (long long i = 0; i < n; ++i) {
                char cle[kMaxCle];
                LireMot(cle, sizeof(cle));
                const bool avant = LireDrapeau() != 0;
                const bool corrige = LireDrapeau() != 0;
                const bool apres = LireDrapeau() != 0;

                const char *verdict = nullptr;
                if (corrige) {
                        ++vraies;
                        verdict = "ENCORE_VRAIE";
                } else if (apres) {
                        ++fausses;
                        verdict = "DEVENUE_FAUSSE";
                } else if (avant) {
                        ++oubliees;
                        verdict = "OUBLIEE";
                } else {
                        ++nouvelles;
                        verdict = "NOUVELLE";
                }

                Ecrire("LIGNE ");
                EcrireEntier(i + 1);
                Ecrire(" ");
                Ecrire(cle);
                Ecrire(" ");
                Ecrire(verdict);
                Ecrire("\n");
        }

        const long long restantes = vraies + oubliees;
        const long long part = restantes > 0 ? (100LL * oubliees) / restantes : 0;

        Ecrire("VRAIES ");
        EcrireEntier(vraies);
        Ecrire("\nFAUSSES ");
        EcrireEntier(fausses);
        Ecrire("\nOUBLIEES ");
        EcrireEntier(oubliees);
        Ecrire("\nNOUVELLES ");
        EcrireEntier(nouvelles);
        Ecrire("\nPART_OUBLIEE ");
        EcrireEntier(part);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
