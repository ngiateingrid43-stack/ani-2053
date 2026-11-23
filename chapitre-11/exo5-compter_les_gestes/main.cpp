#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace {

std::vector<char> g_tampon;
std::vector<char *> g_jetons;
size_t g_courant = 0;

void ChargerEntree() {
        char bloc[1 << 16];
        size_t lu = 0;
        while ((lu = std::fread(bloc, 1, sizeof(bloc), stdin)) > 0)
                g_tampon.insert(g_tampon.end(), bloc, bloc + lu);
        g_tampon.push_back('\0');

        char *p = g_tampon.data();
        char *fin = p + g_tampon.size();
        while (p < fin) {
                while (p < fin && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
                        *p++ = '\0';
                if (p >= fin || *p == '\0')
                        break;
                g_jetons.push_back(p);
                while (p < fin && *p != '\0' && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n')
                        ++p;
        }
}

const char *Jeton() {
        if (g_courant >= g_jetons.size())
                return "";
        return g_jetons[g_courant++];
}

long long JetonNombre() { return std::strtoll(Jeton(), nullptr, 10); }

bool Egal(const char *a, const char *b) { return std::strcmp(a, b) == 0; }

/// Ecrit un nombre de secondes en minutes:secondes, secondes sur deux chiffres.
void EcrireMinutes(long long secondes) {
        const long long minutes = secondes / 60;
        const long long reste = secondes % 60;
        if (reste < 10)
                std::printf("%lld:0%lld", minutes, reste);
        else
                std::printf("%lld:%lld", minutes, reste);
}

struct NkTache {
        const char *nom = "";
        long long minimum = 0;
        long long parJour = 0;
        long long debut = 0;
        long long gestes = 0;
        long long attente = 0;
        bool enCours = false;
};

} // namespace

int main() {
        ChargerEntree();

        NkTache courante;
        bool uneTacheEnCours = false;
        long long totalSecondes = 0;

        for (;;) {
                const char *instant = Jeton();
                if (*instant == '\0')
                        break;
                const long long t = std::strtoll(instant, nullptr, 10);
                const char *quoi = Jeton();

                if (Egal(quoi, "DEBUT")) {
                        courante = NkTache();
                        courante.nom = Jeton();
                        courante.minimum = JetonNombre();
                        courante.parJour = JetonNombre();
                        courante.debut = t;
                        courante.enCours = true;
                        uneTacheEnCours = true;
                } else if (Egal(quoi, "FIN")) {
                        const char *nom = Jeton();
                        if (!uneTacheEnCours)
                                continue;
                        courante.enCours = false;
                        uneTacheEnCours = false;

                        const long long enTrop = courante.gestes > courante.minimum ? courante.gestes - courante.minimum : 0;
                        const long long duree = t - courante.debut;
                        const long long millisecondesJour = courante.attente * courante.parJour;
                        const long long secondesJour = millisecondesJour / 1000;

                        std::printf("%s GESTES %lld EN_TROP %lld DUREE %lld ATTENTE %lld JOUR ", nom,
                                    courante.gestes, enTrop, duree, courante.attente);
                        EcrireMinutes(secondesJour);
                        std::printf("\n");
                        totalSecondes += secondesJour;
                } else if (Egal(quoi, "ATTENTE")) {
                        const long long d = JetonNombre();
                        if (uneTacheEnCours)
                                courante.attente += d;
                } else if (Egal(quoi, "TOUCHE")) {
                        Jeton(); // la touche, non utilisee
                        if (uneTacheEnCours)
                                ++courante.gestes;
                } else if (Egal(quoi, "CLIC") || Egal(quoi, "MOLETTE") || Egal(quoi, "GLISSE")) {
                        if (uneTacheEnCours)
                                ++courante.gestes;
                }
        }

        if (uneTacheEnCours)
                std::printf("%s INACHEVEE\n", courante.nom);

        std::printf("TOTAL_JOUR ");
        EcrireMinutes(totalSecondes);
        std::printf("\n");
        return 0;
}
