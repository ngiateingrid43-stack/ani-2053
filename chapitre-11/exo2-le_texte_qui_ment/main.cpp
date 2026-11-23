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

long JetonNombre() { return std::strtol(Jeton(), nullptr, 10); }

bool Egal(const char *a, const char *b) { return std::strcmp(a, b) == 0; }

class NkTable {
      public:
        NkTable() : m_cases(1u << 20, -1) {}

        int Rang(const char *nom) {
                size_t i = Empreinte(nom) & (m_cases.size() - 1);
                while (m_cases[i] >= 0) {
                        if (Egal(m_noms[static_cast<size_t>(m_cases[i])], nom))
                                return m_cases[i];
                        i = (i + 1) & (m_cases.size() - 1);
                }
                m_noms.push_back(nom);
                const int rang = static_cast<int>(m_noms.size()) - 1;
                m_cases[i] = rang;
                return rang;
        }

        int Chercher(const char *nom) const {
                size_t i = Empreinte(nom) & (m_cases.size() - 1);
                while (m_cases[i] >= 0) {
                        if (Egal(m_noms[static_cast<size_t>(m_cases[i])], nom))
                                return m_cases[i];
                        i = (i + 1) & (m_cases.size() - 1);
                }
                return -1;
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

        std::vector<int> m_cases;
        std::vector<const char *> m_noms;
};

// Pour une fonction : la suite de ses AJOUTE/RETIRE, dans l'ordre des commits.
struct NkChangement {
        int commit;
        bool present;
};

class NkFonction {
      public:
        std::vector<NkChangement> changements;

        bool Presente(int commit) const {
                size_t bas = 0, haut = changements.size();
                while (bas < haut) { // dernier changement au plus tard au commit
                        const size_t milieu = (bas + haut) / 2;
                        if (changements[milieu].commit <= commit)
                                bas = milieu + 1;
                        else
                                haut = milieu;
                }
                if (bas == 0)
                        return false;
                return changements[bas - 1].present;
        }
};

} // namespace

int main() {
        ChargerEntree();

        std::vector<const char *> hashes;
        NkTable nomsDeCommits;
        NkTable nomsDeFonctions;
        std::vector<NkFonction> fonctionsParRang;

        const long nbCommits = JetonNombre();
        for (long c = 0; c < nbCommits; ++c) {
                hashes.push_back(Jeton());
                nomsDeCommits.Rang(hashes.back());
                const bool ajout = Egal(Jeton(), "AJOUTE");
                const long k = JetonNombre();
                for (long j = 0; j < k; ++j) {
                        const int rang = nomsDeFonctions.Rang(Jeton());
                        if (static_cast<size_t>(rang) >= fonctionsParRang.size())
                                fonctionsParRang.resize(static_cast<size_t>(rang) + 1);
                        fonctionsParRang[static_cast<size_t>(rang)].changements.push_back(
                                NkChangement{static_cast<int>(c), ajout});
                }
        }

        struct Annonce {
                int commit;
                bool aVenir;
                int fonction;
        };
        std::vector<Annonce> annonces;

        const long nbAnnonces = JetonNombre();
        for (long i = 0; i < nbAnnonces; ++i) {
                const int commit = nomsDeCommits.Chercher(Jeton());
                const bool aVenir = Egal(Jeton(), "A_VENIR");
                const int fonction = nomsDeFonctions.Rang(Jeton());
                if (static_cast<size_t>(fonction) >= fonctionsParRang.size())
                        fonctionsParRang.resize(static_cast<size_t>(fonction) + 1);
                annonces.push_back(Annonce{commit, aVenir, fonction});
        }

        const int dernier = static_cast<int>(nbCommits) - 1;
        long mensonges = 0;

        for (size_t i = 0; i < annonces.size(); ++i) {
                const Annonce &a = annonces[i];
                const int commitEcriture = a.commit;

                const NkFonction &f = fonctionsParRang[static_cast<size_t>(a.fonction)];
                auto vraie = [&](int commit) {
                        const bool presente = f.Presente(commit);
                        return a.aVenir ? !presente : presente;
                };

                if (vraie(dernier)) {
                        std::printf("ANNONCE %u VRAI\n", static_cast<unsigned>(i + 1));
                        continue;
                }

                int debut = dernier;
                while (debut > commitEcriture && !vraie(debut - 1))
                        --debut;

                std::printf("ANNONCE %u MENT %s\n", static_cast<unsigned>(i + 1),
                            hashes[static_cast<size_t>(debut)]);
                ++mensonges;
        }

        std::printf("MENSONGES %ld\n", mensonges);
        return 0;
}
