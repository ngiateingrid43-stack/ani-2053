#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace {

// Toute l'entree est lue une fois, puis decoupee en jetons : les cles sont
// pointees dans le tampon, jamais recopiees.
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

long JetonNombre() {
        return std::strtol(Jeton(), nullptr, 10);
}

// Ensemble de cles en table ouverte, sans copie : on garde les pointeurs.
const size_t kTable = 1u << 20;

class NkEnsemble {
      public:
        NkEnsemble() : m_cases(kTable, nullptr) {}

        void Inserer(const char *cle) {
                size_t i = Empreinte(cle) & (kTable - 1);
                while (m_cases[i] != nullptr) {
                        if (std::strcmp(m_cases[i], cle) == 0)
                                return;
                        i = (i + 1) & (kTable - 1);
                }
                m_cases[i] = cle;
        }

        bool Contient(const char *cle) const {
                size_t i = Empreinte(cle) & (kTable - 1);
                while (m_cases[i] != nullptr) {
                        if (std::strcmp(m_cases[i], cle) == 0)
                                return true;
                        i = (i + 1) & (kTable - 1);
                }
                return false;
        }

      private:
        static size_t Empreinte(const char *cle) {
                size_t h = 1469598103934665603ull;
                while (*cle != '\0') {
                        h ^= static_cast<unsigned char>(*cle++);
                        h *= 1099511628211ull;
                }
                return h;
        }

        std::vector<const char *> m_cases;
};

// Lit « MOT nombre » puis le nombre de cles. `ordre` garde les cles lues.
void LireListe(const char *motAttendu, NkEnsemble &ensemble, std::vector<const char *> &ordre) {
        const char *mot = Jeton();
        long n = 0;
        if (std::strcmp(mot, motAttendu) == 0)
                n = JetonNombre();
        else
                n = std::strtol(mot, nullptr, 10);
        if (n < 0)
                n = 0;
        for (long i = 0; i < n; ++i) {
                const char *cle = Jeton();
                if (*cle == '\0')
                        break;
                ensemble.Inserer(cle);
                ordre.push_back(cle);
        }
}

} // namespace

int main() {
        ChargerEntree();

        NkEnsemble corrigees;
        NkEnsemble vuesApres;
        NkEnsemble vuesAvant;
        std::vector<const char *> avant;
        std::vector<const char *> apres;
        std::vector<const char *> ordreCorrigees;
        LireListe("AVANT", vuesAvant, avant);
        LireListe("CORRIGE", corrigees, ordreCorrigees);
        LireListe("APRES", vuesApres, apres);

        long vraies = 0, fausses = 0, oubliees = 0, nouvelles = 0;
        for (size_t i = 0; i < avant.size(); ++i) {
                const char *verdict = nullptr;
                if (corrigees.Contient(avant[i])) {
                        verdict = "DEVENUE_FAUSSE";
                        ++fausses;
                } else if (vuesApres.Contient(avant[i])) {
                        verdict = "ENCORE_VRAIE";
                        ++vraies;
                } else {
                        verdict = "OUBLIEE";
                        ++oubliees;
                }
                std::printf("%u %s %s\n", static_cast<unsigned>(i + 1), avant[i], verdict);
        }

        for (size_t i = 0; i < apres.size(); ++i)
                if (!vuesAvant.Contient(apres[i]))
                        ++nouvelles;

        const long toujoursReelles = vraies + oubliees;
        const long part = toujoursReelles > 0 ? (100 * oubliees) / toujoursReelles : 0;

        std::printf("VRAIES %ld\n", vraies);
        std::printf("FAUSSES %ld\n", fausses);
        std::printf("OUBLIEES %ld\n", oubliees);
        std::printf("NOUVELLES %ld\n", nouvelles);
        std::printf("PART_OUBLIEE %ld\n", part);
        return 0;
}
