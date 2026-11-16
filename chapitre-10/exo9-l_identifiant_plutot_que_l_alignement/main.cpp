#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxOperations = 200000;

char gBuffer[1 << 20];
int gAt = 0;
int gLen = 0;

void Remplir() {
        if (gAt < gLen)
                return;
        gLen = (int)fread(gBuffer, 1, sizeof(gBuffer), stdin);
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

long long LireEntier() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        return strtoll(mot, nullptr, 10);
}

int gIdentifiants[kMaxOperations];
int gTaille = 0;
int gProchain = 1;
long long gOccasions = 0;
long long gRepercutes = 0;
long long gRecherches = 0;
int gIndex[100000];

void Retirer(int index) {
        for (int i = index; i < gTaille - 1; ++i)
                gIdentifiants[i] = gIdentifiants[i + 1];
        --gTaille;
}

void Echanger(int a, int b) {
        const int tmp = gIdentifiants[a];
        gIdentifiants[a] = gIdentifiants[b];
        gIdentifiants[b] = tmp;
}

void Traiter() {
        gTaille = 0;
        gProchain = 1;
        int n = (int)LireEntier();
        if (n < 0)
                n = 0;
        if (n > kMaxOperations)
                n = kMaxOperations;

        for (int k = 0; k < n; ++k) {
                char commande[32];
                if (LireMot(commande, sizeof(commande)) == 0)
                        break;

                if (strcmp(commande, "AJOUTE") == 0) {
                        gIdentifiants[gTaille] = gProchain;
                        ++gProchain;
                        ++gTaille;
                } else if (strcmp(commande, "SUPPRIME") == 0) {
                        const int c = (int)LireEntier();
                        int nombre = c < 0 ? 0 : c;
                        if (nombre > 100000)
                                nombre = 100000;
                        for (int i = 0; i < nombre; ++i)
                                gIndex[i] = (int)LireEntier();
                        for (int i = nombre - 1; i >= 0; --i) {
                                if (gIndex[i] >= 0 && gIndex[i] < gTaille)
                                        Retirer(gIndex[i]);
                        }
                        if (nombre > 0) {
                                ++gOccasions;
                                gRepercutes += 2 * (long long)nombre;
                        }
                } else if (strcmp(commande, "MONTE") == 0 || strcmp(commande, "DESCEND") == 0) {
                        const int i = (int)LireEntier();
                        const int voisin = strcmp(commande, "MONTE") == 0 ? i + 1 : i - 1;
                        if (i >= 0 && i < gTaille && voisin >= 0 && voisin < gTaille) {
                                Echanger(i, voisin);
                                ++gOccasions;
                                gRepercutes += 2;
                        }
                } else if (strcmp(commande, "DESSINE") == 0) {
                        gRecherches += (long long)gTaille;
                }
        }

        fputs("ORDRE ", stdout);
        if (gTaille == 0) {
                fputs("AUCUN", stdout);
        } else {
                for (int i = 0; i < gTaille; ++i) {
                        if (i > 0)
                                fputc(' ', stdout);
                        char nombre[16];
                        int m = 0;
                        int x = gIdentifiants[i];
                        if (x == 0) {
                                nombre[m++] = '0';
                        } else {
                                char tmp[16];
                                int t = 0;
                                while (x > 0) {
                                        tmp[t++] = (char)('0' + x % 10);
                                        x /= 10;
                                }
                                while (t > 0)
                                        nombre[m++] = tmp[--t];
                        }
                        nombre[m] = 0;
                        fputs(nombre, stdout);
                }
        }
        char ligne[128];
        snprintf(ligne, sizeof(ligne), "\nOCCASIONS %lld\nREPERCUTES %lld\nRECHERCHES %lld\n", gOccasions, gRepercutes,
                 gRecherches);
        fputs(ligne, stdout);
}

} // namespace

int main() {
        Traiter();
        return 0;
}
