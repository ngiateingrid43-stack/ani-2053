#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxLignes = 200000;
const int kMaxMot = 32;
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

struct Trait {
        char verbe[kMaxMot];
        char cible[kMaxMot];
};

struct Demande {
        char verbe[kMaxMot];
        char cible[kMaxMot];
};

Trait gTraits[kMaxLignes];
Demande gDemandes[kMaxLignes];
bool gConsomme[kMaxLignes];

void Traiter() {
        int n = LireInt();
        if (n < 0)
                n = 0;
        if (n > kMaxLignes)
                n = kMaxLignes;
        for (int i = 0; i < n; ++i) {
                LireMot(gTraits[i].verbe, sizeof(gTraits[i].verbe));
                LireMot(gTraits[i].cible, sizeof(gTraits[i].cible));
        }

        int m = LireInt();
        if (m < 0)
                m = 0;
        if (m > kMaxLignes)
                m = kMaxLignes;
        for (int k = 0; k < m; ++k) {
                LireMot(gDemandes[k].verbe, sizeof(gDemandes[k].verbe));
                LireMot(gDemandes[k].cible, sizeof(gDemandes[k].cible));
        }

        for (int i = 0; i < n; ++i)
                gConsomme[i] = false;
        int curseur = 0;
        int restants = n;

        long long justes = 0;
        long long riens = 0;
        long long trahis = 0;

        for (int k = 0; k < m; ++k) {
                const Demande &d = gDemandes[k];
                const bool muet = strcmp(d.verbe, "RIEN") == 0;

                Ecrire("ANNULE ");
                EcrireEntier(k + 1);
                Ecrire(" ATTENDU ");
                Ecrire(d.verbe);
                if (!muet) {
                        Ecrire(" ");
                        Ecrire(d.cible);
                }

                if (restants == 0) {
                        Ecrire(" OBTENU AUCUN ");
                        if (muet) {
                                ++riens;
                                Ecrire("RIEN\n");
                        } else {
                                ++trahis;
                                Ecrire("TRAHI\n");
                        }
                        continue;
                }

                while (curseur < n && gConsomme[curseur])
                        ++curseur;
                if (curseur >= n)
                        break;

                const Trait &t = gTraits[curseur];
                gConsomme[curseur] = true;
                --restants;

                Ecrire(" OBTENU TRAIT ");
                EcrireEntier(curseur + 1);
                Ecrire(" ");

                const bool accord = !muet && strcmp(d.verbe, t.verbe) == 0 && strcmp(d.cible, t.cible) == 0;
                if (accord) {
                        ++justes;
                        Ecrire("JUSTE\n");
                } else {
                        ++trahis;
                        Ecrire("TRAHI\n");
                }
        }

        long long annulables = 0;
        for (int i = 0; i < n; ++i) {
                if (gConsomme[i])
                        continue;
                for (int k = 0; k < m; ++k) {
                        if (strcmp(gDemandes[k].verbe, gTraits[i].verbe) == 0 &&
                            strcmp(gDemandes[k].cible, gTraits[i].cible) == 0) {
                                ++annulables;
                                break;
                        }
                }
        }

        Ecrire("JUSTES ");
        EcrireEntier(justes);
        Ecrire("\nRIENS ");
        EcrireEntier(riens);
        Ecrire("\nTRAHIS ");
        EcrireEntier(trahis);
        Ecrire("\nANNULABLES ");
        EcrireEntier(annulables);
        Ecrire(" SUR ");
        EcrireEntier(n);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
