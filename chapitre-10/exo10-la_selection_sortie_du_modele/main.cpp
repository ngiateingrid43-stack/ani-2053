#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxLignes = 200000;

struct Image {
        char nom[32];
        bool choisiDansItem;
        int identifiant;
};

const int kConceptions = 3;

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

int LireInt() {
        char mot[32];
        LireMot(mot, sizeof(mot));
        return atoi(mot);
}

Image gPlanches[kConceptions][kMaxLignes];
int gTailles[kConceptions];

bool gGardesIndex[kMaxLignes];
bool gGardesId[kMaxLignes];
int gIdentifiantSuivant = 1;

void Ajouter(int conception, const char *nom) {
        Image &img = gPlanches[conception][gTailles[conception]];
        int n = 0;
        while (nom[n] != 0 && n < 31) {
                img.nom[n] = nom[n];
                ++n;
        }
        img.nom[n] = 0;
        img.choisiDansItem = false;
        img.identifiant = gIdentifiantSuivant;
        ++gTailles[conception];
}

void Retirer(int conception, int index) {
        for (int i = index; i < gTailles[conception] - 1; ++i)
                gPlanches[conception][i] = gPlanches[conception][i + 1];
        --gTailles[conception];
}

void Echanger(Image *planche, int a, int b) {
        const Image tmp = planche[a];
        planche[a] = planche[b];
        planche[b] = tmp;
}

bool EstChoisi(int conception, int index) {
        const Image &img = gPlanches[conception][index];
        if (conception == 0)
                return img.choisiDansItem;
        if (conception == 1)
                return gGardesIndex[index];
        return gGardesId[img.identifiant];
}

void EcrireListe(const char *prefixe, int conception, bool listeDeLaSelection) {
        fputs(prefixe, stdout);
        bool premier = true;
        for (int i = 0; i < gTailles[conception]; ++i) {
                if (listeDeLaSelection && !EstChoisi(conception, i))
                        continue;
                if (!premier)
                        fputc(' ', stdout);
                fputs(gPlanches[conception][i].nom, stdout);
                premier = false;
        }
        if (premier)
                fputs("AUCUN", stdout);
        fputc('\n', stdout);
}

void Traiter() {
        for (int c = 0; c < kConceptions; ++c)
                gTailles[c] = 0;
        for (int i = 0; i < kMaxLignes; ++i) {
                gGardesIndex[i] = false;
                gGardesId[i] = false;
        }

        char commande[32];
        while (LireMot(commande, sizeof(commande)) > 0) {
                if (strcmp(commande, "AJOUTE") == 0) {
                        char nom[32];
                        LireMot(nom, sizeof(nom));
                        for (int c = 0; c < kConceptions; ++c)
                                Ajouter(c, nom);
                        ++gIdentifiantSuivant;
                } else if (strcmp(commande, "CLIC") == 0) {
                        const int i = LireInt();
                        for (int c = 0; c < kConceptions; ++c) {
                                for (int k = 0; k < gTailles[c]; ++k)
                                        gPlanches[c][k].choisiDansItem = false;
                                for (int k = 0; k < kMaxLignes; ++k) {
                                        gGardesIndex[k] = false;
                                        gGardesId[k] = false;
                                }
                                if (i >= 0 && i < gTailles[c]) {
                                        gPlanches[c][i].choisiDansItem = true;
                                        gGardesIndex[i] = true;
                                        gGardesId[gPlanches[c][i].identifiant] = true;
                                }
                        }
                } else if (strcmp(commande, "AJOUTE_SEL") == 0) {
                        const int i = LireInt();
                        for (int c = 0; c < kConceptions; ++c) {
                                if (i >= 0 && i < gTailles[c]) {
                                        gPlanches[c][i].choisiDansItem = true;
                                        gGardesIndex[i] = true;
                                        gGardesId[gPlanches[c][i].identifiant] = true;
                                }
                        }
                } else if (strcmp(commande, "ECHANGE") == 0) {
                        const int i = LireInt();
                        const int j = LireInt();
                        for (int c = 0; c < kConceptions; ++c) {
                                if (i >= 0 && i < gTailles[c] && j >= 0 && j < gTailles[c])
                                        Echanger(gPlanches[c], i, j);
                        }
                } else if (strcmp(commande, "RETIRE") == 0) {
                        const int i = LireInt();
                        for (int c = 0; c < kConceptions; ++c) {
                                if (i >= 0 && i < gTailles[c])
                                        Retirer(c, i);
                        }
                } else if (strcmp(commande, "SUPPRIME") == 0) {
                        for (int c = 0; c < kConceptions; ++c) {
                                for (int k = gTailles[c] - 1; k >= 0; --k)
                                        if (EstChoisi(c, k))
                                                Retirer(c, k);
                                for (int k = 0; k < gTailles[c]; ++k)
                                        gPlanches[c][k].choisiDansItem = false;
                        }
                        for (int k = 0; k < kMaxLignes; ++k) {
                                gGardesIndex[k] = false;
                                gGardesId[k] = false;
                        }
                }
        }

        static const char *kPrefixes[kConceptions] = {"DANS_ITEM", "PAR_INDEX", "PAR_ID"};
        for (int c = 0; c < kConceptions; ++c) {
                char prefixe[48];
                snprintf(prefixe, sizeof(prefixe), "%s PLANCHE ", kPrefixes[c]);
                EcrireListe(prefixe, c, false);
                snprintf(prefixe, sizeof(prefixe), "%s SELECTION ", kPrefixes[c]);
                EcrireListe(prefixe, c, true);
        }

        char fautives[128];
        int longueur = 0;
        bool aucune = true;
        for (int c = 1; c < kConceptions; ++c) {
                bool differente = gTailles[c] != gTailles[0];
                if (!differente) {
                        for (int i = 0; i < gTailles[c]; ++i) {
                                if (strcmp(gPlanches[c][i].nom, gPlanches[0][i].nom) != 0) {
                                        differente = true;
                                        break;
                                }
                        }
                }
                if (!differente) {
                        for (int i = 0; i < gTailles[c]; ++i) {
                                if (EstChoisi(c, i) != EstChoisi(0, i)) {
                                        differente = true;
                                        break;
                                }
                        }
                }
                if (differente) {
                        const int n = snprintf(fautives + longueur, sizeof(fautives) - (size_t)longueur, "%s%s",
                                               aucune ? "" : " ", kPrefixes[c]);
                        longueur += n;
                        aucune = false;
                }
        }
        fputs("FAUX ", stdout);
        if (aucune)
                fputs("AUCUN", stdout);
        else
                fputs(fautives, stdout);
        fputc('\n', stdout);
}

} // namespace

int main() {
        Traiter();
        return 0;
}
