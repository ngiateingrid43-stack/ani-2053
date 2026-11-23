#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxFaits = 50000;
const int kMaxObservations = 200000;
const int kMaxCitations = 100000;
const int kMaxNom = 32;
const int kMaxCites = 16;
const int kFinSeance = 20 * 60;
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

int LireHeure() {
        char mot[16];
        LireMot(mot, sizeof(mot));
        int m = 0;
        int s = 0;
        const char *p = mot;
        while (*p >= '0' && *p <= '9')
                m = m * 10 + (*p++ - '0');
        if (*p == ':') {
                ++p;
                while (*p >= '0' && *p <= '9')
                        s = s * 10 + (*p++ - '0');
        }
        return m * 60 + s;
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

void EcrireHeure(int secondes) {
        EcrireEntier(secondes / 60);
        Ecrire(":");
        const int s = secondes % 60;
        if (s < 10)
                Ecrire("0");
        EcrireEntier(s);
}

struct Fait {
        char nom[kMaxNom];
        int premiereVue;
        int fois;
        int rang;
};

Fait gFaits[kMaxFaits];
int gNbFaits = 0;

int TrouverFait(const char *nom) {
        for (int i = 0; i < gNbFaits; ++i)
                if (strcmp(gFaits[i].nom, nom) == 0)
                        return i;
        return -1;
}

int IndexFait(const char *nom) {
        const int trouve = TrouverFait(nom);
        if (trouve >= 0)
                return trouve;
        if (gNbFaits >= kMaxFaits)
                return -1;
        Fait &f = gFaits[gNbFaits];
        strncpy(f.nom, nom, sizeof(f.nom) - 1);
        f.nom[sizeof(f.nom) - 1] = 0;
        f.premiereVue = -1;
        f.fois = 0;
        f.rang = gNbFaits;
        return gNbFaits++;
}

bool VuAvant(int fait, int heure) {
        const Fait &f = gFaits[fait];
        return f.premiereVue >= 0 && f.premiereVue <= heure;
}

struct Citation {
        int heure;
        int nb;
        char texte[kMaxCites][kMaxNom];
};

Citation gCitations[kMaxCitations];
int gNbCitations = 0;

int gOrdre[kMaxFaits];

void TrierFaits() {
        for (int i = 0; i < gNbFaits; ++i)
                gOrdre[i] = i;
        for (int i = 1; i < gNbFaits; ++i) {
                const int cle = gOrdre[i];
                int j = i - 1;
                while (j >= 0) {
                        const Fait &a = gFaits[gOrdre[j]];
                        const Fait &b = gFaits[cle];
                        bool avant = false;
                        if (a.fois != b.fois)
                                avant = a.fois < b.fois;
                        else if (a.premiereVue != b.premiereVue)
                                avant = a.premiereVue > b.premiereVue;
                        else
                                avant = a.rang > b.rang;
                        if (!avant)
                                break;
                        gOrdre[j + 1] = gOrdre[j];
                        --j;
                }
                gOrdre[j + 1] = cle;
        }
}

void Traiter() {
        const int nbObs = LireInt();
        int obs = nbObs < 0 ? 0 : (nbObs > kMaxObservations ? kMaxObservations : nbObs);
        for (int i = 0; i < nbObs; ++i) {
                char nom[kMaxNom];
                LireMot(nom, sizeof(nom));
                const int heure = LireHeure();
                if (i >= obs)
                        continue;
                const int f = IndexFait(nom);
                if (f < 0)
                        continue;
                ++gFaits[f].fois;
                if (gFaits[f].premiereVue < 0 || heure < gFaits[f].premiereVue)
                        gFaits[f].premiereVue = heure;
        }

        const int nbCit = LireInt();
        const int cit = nbCit < 0 ? 0 : (nbCit > kMaxCitations ? kMaxCitations : nbCit);
        for (int i = 0; i < nbCit; ++i) {
                const int heure = LireHeure();
                int nombre = LireInt();
                if (nombre < 0)
                        nombre = 0;
                Citation &c = gCitations[gNbCitations];
                c.heure = heure;
                c.nb = 0;
                for (int k = 0; k < nombre; ++k) {
                        char nom[kMaxNom];
                        LireMot(nom, sizeof(nom));
                        if (k >= kMaxCites)
                                continue;
                        strncpy(c.texte[c.nb], nom, kMaxNom - 1);
                        c.texte[c.nb][kMaxNom - 1] = 0;
                        ++c.nb;
                }
                if (i < cit)
                        ++gNbCitations;
                else
                        c.nb = 0;
        }

        long long dits = 0;
        long long nonDits = 0;
        long long hors = 0;
        long long sansVu = 0;

        for (int i = 0; i < gNbCitations; ++i) {
                const Citation &c = gCitations[i];

                Ecrire("CITATION ");
                EcrireEntier(i + 1);
                Ecrire(" ");
                EcrireHeure(c.heure);
                Ecrire(" ");

                if (c.heure > kFinSeance) {
                        ++hors;
                        Ecrire("HORS_SEANCE\n");
                        continue;
                }

                bool auMoinsUnVu = false;
                bool auMoinsUnInconnu = false;
                for (int k = 0; k < c.nb; ++k) {
                        const int f = TrouverFait(c.texte[k]);
                        if (f < 0)
                                auMoinsUnInconnu = true;
                        else if (VuAvant(f, c.heure))
                                auMoinsUnVu = true;
                }

                if (c.nb == 0) {
                        ++nonDits;
                        Ecrire("NON_DIT\n");
                        continue;
                }

                if (!auMoinsUnVu) {
                        ++sansVu;
                        Ecrire("DIT_SANS_VU\n");
                        continue;
                }

                if (auMoinsUnInconnu) {
                        ++nonDits;
                        Ecrire("NON_DIT\n");
                        continue;
                }

                bool tousVus = true;
                for (int k = 0; k < c.nb; ++k) {
                        const int f = TrouverFait(c.texte[k]);
                        if (f < 0 || !VuAvant(f, c.heure))
                                tousVus = false;
                }

                if (tousVus) {
                        ++dits;
                        Ecrire("DIT\n");
                } else {
                        ++nonDits;
                        Ecrire("NON_DIT\n");
                }
        }

        TrierFaits();
        for (int i = 0; i < gNbFaits; ++i) {
                const Fait &f = gFaits[gOrdre[i]];
                Ecrire("FAIT ");
                Ecrire(f.nom);
                Ecrire(" FOIS ");
                EcrireEntier(f.fois);
                Ecrire(" PREMIERE ");
                if (f.premiereVue < 0)
                        Ecrire("AUCUNE\n");
                else {
                        EcrireHeure(f.premiereVue);
                        Ecrire("\n");
                }
        }

        const long long total = dits + nonDits;
        const long long part = total > 0 ? (100LL * nonDits) / total : 0;

        Ecrire("DITS ");
        EcrireEntier(dits);
        Ecrire("\nNON_DITS ");
        EcrireEntier(nonDits);
        Ecrire("\nPART_NON_DITE ");
        EcrireEntier(part);
        Ecrire("\nDIT_SANS_VU ");
        EcrireEntier(sansVu);
        Ecrire("\nHORS_SEANCE ");
        EcrireEntier(hors);
        Ecrire("\n");

        if (sansVu == 0 && nonDits == 0)
                Ecrire("AUCUN\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
