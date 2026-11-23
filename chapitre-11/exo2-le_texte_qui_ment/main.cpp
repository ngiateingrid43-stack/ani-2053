#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxCommits = 50000;
const int kMaxMots = 200000;
const int kMaxMot = 40;
const int kMaxId = 24;
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

struct Commit {
        char id[kMaxId];
        char idParent[kMaxId];
        int parent;
        int profondeur;
        int premierMot;
        int nbMots;
};

struct Mot {
        char texte[kMaxMot];
        int commit;
};

Commit gCommits[kMaxCommits];
Mot gMots[kMaxMots];
int gNbCommits = 0;
int gNbMots = 0;

int Trouver(const char *id) {
        for (int i = 0; i < gNbCommits; ++i)
                if (strcmp(gCommits[i].id, id) == 0)
                        return i;
        return -1;
}

void CalculerProfondeurs() {
        for (int i = 0; i < gNbCommits; ++i) {
                int d = 0;
                int p = gCommits[i].parent;
                int garde = 0;
                while (p >= 0 && garde <= gNbCommits) {
                        ++d;
                        p = gCommits[p].parent;
                        ++garde;
                }
                gCommits[i].profondeur = d;
        }
}

bool Contient(int commit, const char *mot) {
        const Commit &c = gCommits[commit];
        for (int k = 0; k < c.nbMots; ++k)
                if (strcmp(gMots[c.premierMot + k].texte, mot) == 0)
                        return true;
        return false;
}

bool Present(int commit, const char *mot) {
        int p = commit;
        int garde = 0;
        while (p >= 0 && garde <= gNbCommits) {
                if (Contient(p, mot))
                        return true;
                p = gCommits[p].parent;
                ++garde;
        }
        return false;
}

int DernierPorteur(const char *mot) {
        int meilleur = -1;
        for (int i = 0; i < gNbCommits; ++i) {
                if (!Contient(i, mot))
                        continue;
                if (meilleur < 0 || gCommits[i].profondeur > gCommits[meilleur].profondeur)
                        meilleur = i;
        }
        return meilleur;
}

void Traiter() {
        gNbCommits = LireInt();
        if (gNbCommits < 0)
                gNbCommits = 0;
        if (gNbCommits > kMaxCommits)
                gNbCommits = kMaxCommits;

        for (int i = 0; i < gNbCommits; ++i) {
                Commit &c = gCommits[i];
                LireMot(c.id, sizeof(c.id));
                LireMot(c.idParent, sizeof(c.idParent));
                c.parent = -1;
                c.premierMot = gNbMots;
                c.nbMots = LireInt();
                if (c.nbMots < 0)
                        c.nbMots = 0;
                for (int k = 0; k < c.nbMots; ++k) {
                        if (gNbMots >= kMaxMots)
                                break;
                        LireMot(gMots[gNbMots].texte, sizeof(gMots[gNbMots].texte));
                        gMots[gNbMots].commit = i;
                        ++gNbMots;
                }
        }

        for (int i = 0; i < gNbCommits; ++i) {
                const char *pere = gCommits[i].idParent;
                if (pere[0] == '-' && pere[1] == 0)
                        continue;
                gCommits[i].parent = Trouver(pere);
        }

        CalculerProfondeurs();

        const int m = LireInt();
        long long mensonges = 0;
        for (int k = 0; k < m; ++k) {
                char mot[kMaxMot];
                char id[kMaxId];
                LireMot(mot, sizeof(mot));
                LireMot(id, sizeof(id));
                const int c = Trouver(id);

                bool vrai = false;
                if (c >= 0)
                        vrai = Present(c, mot);

                Ecrire("ANNONCE ");
                EcrireEntier(k + 1);
                if (vrai) {
                        Ecrire(" VRAI\n");
                } else {
                        ++mensonges;
                        Ecrire(" MENT ");
                        const int h = DernierPorteur(mot);
                        if (h < 0)
                                Ecrire("AUCUN");
                        else
                                Ecrire(gCommits[h].id);
                        Ecrire("\n");
                }
        }

        Ecrire("MENSONGES ");
        EcrireEntier(mensonges);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
