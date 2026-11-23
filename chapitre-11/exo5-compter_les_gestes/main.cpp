#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const int kMaxTaches = 100000;
const int kMaxNom = 32;
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

void EcrireDuree(long long ms) {
        if (ms < 0)
                ms = 0;
        const long long secondes = ms / 1000;
        const long long minutes = secondes / 60;
        const long long reste = secondes % 60;
        EcrireEntier(minutes);
        Ecrire(":");
        if (reste < 10)
                Ecrire("0");
        EcrireEntier(reste);
}

struct Tache {
        char nom[kMaxNom];
        long long debut;
        long long fin;
        int gestes;
        int enTrop;
};

Tache gTaches[kMaxTaches];

void Traiter() {
        int n = LireInt();
        if (n < 0)
                n = 0;
        if (n > kMaxTaches)
                n = kMaxTaches;

        for (int i = 0; i < n; ++i) {
                LireMot(gTaches[i].nom, sizeof(gTaches[i].nom));
                gTaches[i].debut = LireEntier();
                gTaches[i].fin = LireEntier();
                gTaches[i].gestes = LireInt();
                gTaches[i].enTrop = LireInt();
        }

        long long finPrecedente = 0;
        long long finDuJour = 0;

        for (int i = 0; i < n; ++i) {
                const Tache &t = gTaches[i];
                long long duree = t.fin - t.debut;
                if (duree < 0)
                        duree = 0;
                long long attente = t.debut - finPrecedente;
                if (attente < 0)
                        attente = 0;

                Ecrire(t.nom);
                Ecrire(" GESTES ");
                EcrireEntier(t.gestes);
                Ecrire(" EN_TROP ");
                EcrireEntier(t.enTrop);
                Ecrire(" DUREE ");
                EcrireEntier(duree);
                Ecrire(" ATTENTE ");
                EcrireEntier(attente);
                Ecrire(" JOUR ");
                EcrireDuree(t.fin);
                Ecrire("\n");

                finPrecedente = t.fin;
                if (t.fin > finDuJour)
                        finDuJour = t.fin;
        }

        Ecrire("TOTAL_JOUR ");
        EcrireDuree(finDuJour);
        Ecrire("\n");
}

} // namespace

int main() {
        Traiter();
        return 0;
}
