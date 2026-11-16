#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxLignes = 200000;
const int kMaxOperations = 200000;

struct Liste {
        char (*noms)[64];
        int taille;

        void Init(char (*stockage)[64]) {
                noms = stockage;
                taille = 0;
        }

        void Ajouter(const char *nom) {
                int n = 0;
                while (nom[n] != 0 && n < 63) {
                        noms[taille][n] = nom[n];
                        ++n;
                }
                noms[taille][n] = 0;
                ++taille;
        }

        void Retirer(int index) {
                for (int i = index; i + 1 < taille; ++i) {
                        for (int c = 0; c < 64; ++c)
                                noms[i][c] = noms[i + 1][c];
                }
                --taille;
        }

        void Echanger(int a, int b) {
                char tmp[64];
                for (int c = 0; c < 64; ++c)
                        tmp[c] = noms[a][c];
                for (int c = 0; c < 64; ++c)
                        noms[a][c] = noms[b][c];
                for (int c = 0; c < 64; ++c)
                        noms[b][c] = tmp[c];
        }

        bool Existe(int index) const {
                return index >= 0 && index < taille;
        }

        const char *Nom(int index) const {
                if (!Existe(index))
                        return "-";
                return noms[index];
        }
};

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

int LireLigne(char *dst, int cap) {
        int n = 0;
        char c = LireChar();
        while (c != 0 && c != '\n') {
                if (c != '\r' && n < cap - 1)
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

bool LigneCite(const char *ligne, const char *nom) {
        const int longueur = (int)strlen(nom);
        for (int i = 0; ligne[i] != 0; ++i) {
                bool pareil = true;
                for (int k = 0; k < longueur; ++k) {
                        if (ligne[i + k] != nom[k]) {
                                pareil = false;
                                break;
                        }
                }
                if (pareil) {
                        const char avant = i > 0 ? ligne[i - 1] : ' ';
                        const char apres = ligne[i + longueur];
                        const bool avantOk = avant == ' ' || avant == '\t';
                        const bool apresOk = apres == 0 || apres == ' ' || apres == '\t';
                        if (avantOk && apresOk)
                                return true;
                }
        }
        return false;
}

int gIndex[100000];

char gModeleStock[kMaxLignes][64];
char gTexturesStock[kMaxLignes][64];
char gSourcesStock[kMaxLignes][64];

Liste gModele;
Liste gTextures;
Liste gSources;

bool gAjouteTextures = false, gAjouteSources = false;
bool gSupprimeTextures = false, gSupprimeSources = false;
bool gEchangeTextures = false, gEchangeSources = false;

void Traiter() {
        char ligne[256];

        LireLigne(ligne, sizeof(ligne));
        gAjouteTextures = LigneCite(ligne, "textures");
        gAjouteSources = LigneCite(ligne, "sources");

        LireLigne(ligne, sizeof(ligne));
        gSupprimeTextures = LigneCite(ligne, "textures");
        gSupprimeSources = LigneCite(ligne, "sources");

        LireLigne(ligne, sizeof(ligne));
        gEchangeTextures = LigneCite(ligne, "textures");
        gEchangeSources = LigneCite(ligne, "sources");

        gModele.Init(gModeleStock);
        gTextures.Init(gTexturesStock);
        gSources.Init(gSourcesStock);

        int n = LireInt();
        if (n < 0)
                n = 0;
        if (n > kMaxOperations)
                n = kMaxOperations;

        for (int k = 0; k < n; ++k) {
                char commande[32];
                if (LireMot(commande, sizeof(commande)) == 0)
                        break;

                if (strcmp(commande, "AJOUTE") == 0) {
                        char nom[64];
                        LireMot(nom, sizeof(nom));
                        gModele.Ajouter(nom);
                        if (gAjouteTextures)
                                gTextures.Ajouter(nom);
                        if (gAjouteSources)
                                gSources.Ajouter(nom);
                } else if (strcmp(commande, "SUPPRIME") == 0) {
                        const int c = LireInt();
                        int nombre = c < 0 ? 0 : c;
                        if (nombre > 100000)
                                nombre = 100000;
                        for (int i = 0; i < nombre; ++i)
                                gIndex[i] = LireInt();
                        for (int i = nombre - 1; i >= 0; --i) {
                                const int index = gIndex[i];
                                if (!gModele.Existe(index))
                                        continue;
                                gModele.Retirer(index);
                                if (gSupprimeTextures && gTextures.Existe(index))
                                        gTextures.Retirer(index);
                                if (gSupprimeSources && gSources.Existe(index))
                                        gSources.Retirer(index);
                        }
                } else if (strcmp(commande, "MONTE") == 0 || strcmp(commande, "DESCEND") == 0) {
                        const int i = LireInt();
                        const int voisin = strcmp(commande, "MONTE") == 0 ? i + 1 : i - 1;
                        if (!gModele.Existe(i) || !gModele.Existe(voisin))
                                continue;
                        gModele.Echanger(i, voisin);
                        if (gEchangeTextures && gTextures.Existe(i) && gTextures.Existe(voisin))
                                gTextures.Echanger(i, voisin);
                        if (gEchangeSources && gSources.Existe(i) && gSources.Existe(voisin))
                                gSources.Echanger(i, voisin);
                }
        }

        int longueur = gModele.taille;
        if (gTextures.taille > longueur)
                longueur = gTextures.taille;
        if (gSources.taille > longueur)
                longueur = gSources.taille;

        int decales = 0;
        for (int i = 0; i < longueur; ++i) {
                const bool a = gModele.Existe(i);
                const bool b = gTextures.Existe(i);
                const bool c = gSources.Existe(i);
                const char *nm = gModele.Nom(i);
                const char *nt = gTextures.Nom(i);
                const char *ns = gSources.Nom(i);
                const bool decale = !(a && b && c) || strcmp(nm, nt) != 0 || strcmp(nm, ns) != 0;
                if (decale)
                        ++decales;
                printf("%d %s %s %s\n", i, nm, nt, ns);
        }
        printf("DECALES %d\n", decales);
}

} // namespace

int main() {
        Traiter();
        return 0;
}
