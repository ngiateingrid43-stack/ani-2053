#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxItems = 200000;
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

long long LireEntier() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        return strtoll(mot, nullptr, 10);
}

int LireInt() {
        return (int)LireEntier();
}

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

struct Image {
        long long id;
        long long octets;
        int selectionne;
        bool cheminVide;
};

Image gImages[kMaxItems];
char gChemin[512];

void Traiter() {
        const int n = LireInt();
        if (n < 0)
                return;
        int nombre = n < kMaxItems ? n : kMaxItems;
        for (int i = 0; i < nombre; ++i) {
                const long long id = LireEntier();
                for (int champ = 0; champ < 9; ++champ)
                        LireMot(gChemin, sizeof(gChemin));
                const int selectionne = LireInt();
                LireMot(gChemin, sizeof(gChemin));
                const long long octets = LireEntier();
                gImages[i].id = id;
                gImages[i].selectionne = selectionne;
                gImages[i].octets = octets;
                gImages[i].cheminVide = (gChemin[0] == '-' && gChemin[1] == 0);
        }

        long long restent = 0;
        long long changes = 0;
        long long place = 0;
        for (int i = 0; i < nombre; ++i) {
                const Image &img = gImages[i];
                const int k = i + 1;
                if (img.octets == 0) {
                        Ecrire("ITEM ");
                        EcrireEntier(k);
                        Ecrire(" SAUTE\n");
                        continue;
                }
                ++place;
                ++restent;
                bool champs[3];
                champs[0] = (img.id != 0);
                champs[1] = (img.selectionne != 0);
                champs[2] = !img.cheminVide;
                int compte = 0;
                for (int c = 0; c < 3; ++c)
                        if (champs[c])
                                ++compte;
                changes += compte;

                Ecrire("ITEM ");
                EcrireEntier(k);
                Ecrire(" ");
                EcrireEntier(place);
                if (compte == 0) {
                        Ecrire(" AUCUN\n");
                } else {
                        if (champs[0])
                                Ecrire(" id");
                        if (champs[1])
                                Ecrire(" selected");
                        if (champs[2])
                                Ecrire(" sourcePath");
                        Ecrire("\n");
                }
        }
        Ecrire("RESTENT ");
        EcrireEntier(restent);
        Ecrire("\nCHANGES ");
        EcrireEntier(changes);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
