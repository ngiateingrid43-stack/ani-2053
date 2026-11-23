#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

const long kFinSeance = 20 * 60; // vingt minutes, en secondes

// « mm:ss » -> secondes. Un champ mal forme vaut 0, ce qui le range hors seance.
long Secondes(const std::string &texte) {
        const size_t deuxPoints = texte.find(':');
        if (deuxPoints == std::string::npos)
                return 0;
        long minutes = 0, sec = 0;
        for (size_t i = 0; i < deuxPoints; ++i) {
                if (texte[i] < '0' || texte[i] > '9')
                        return 0;
                minutes = minutes * 10 + (texte[i] - '0');
        }
        for (size_t i = deuxPoints + 1; i < texte.size(); ++i) {
                if (texte[i] < '0' || texte[i] > '9')
                        return 0;
                sec = sec * 10 + (texte[i] - '0');
        }
        return minutes * 60 + sec;
}

struct NkContournement {
        std::string nom;
        long fois = 0;
        long premiere = 0;  // instant de la premiere observation dans la seance
        long rangEntree = 0; // ordre d'apparition dans l'entree
        bool dit = false;
};

} // namespace

int main() {
        std::vector<NkContournement> vus;
        std::vector<std::string> cites;
        long horsSeance = 0;

        char tampon[1 << 16];
        std::string mot;
        long n = 0;

        // OBSERVE n, puis n lignes « mm:ss nom ».
        if (std::scanf("%16383s", tampon) == 1) {
                mot = tampon;
                if (mot == "OBSERVE") {
                        if (std::scanf("%ld", &n) != 1)
                                n = 0;
                } else {
                        n = std::strtol(mot.c_str(), nullptr, 10);
                }
        }
        for (long i = 0; i < n; ++i) {
                char instant[64];
                char nom[256];
                if (std::scanf("%63s %255s", instant, nom) != 2)
                        break;
                const long t = Secondes(instant);
                if (t > kFinSeance) {
                        ++horsSeance;
                        continue;
                }
                size_t k = 0;
                for (; k < vus.size(); ++k)
                        if (vus[k].nom == nom)
                                break;
                if (k == vus.size())
                        vus.push_back(NkContournement{nom, 0, t, static_cast<long>(vus.size()), false});
                ++vus[k].fois;
                if (t < vus[k].premiere)
                        vus[k].premiere = t;
        }

        // DIT m, puis m noms.
        long m = 0;
        if (std::scanf("%16383s", tampon) == 1) {
                mot = tampon;
                if (mot == "DIT") {
                        if (std::scanf("%ld", &m) != 1)
                                m = 0;
                } else {
                        m = std::strtol(mot.c_str(), nullptr, 10);
                }
        }
        for (long i = 0; i < m; ++i) {
                char nom[256];
                if (std::scanf("%255s", nom) != 1)
                        break;
                cites.push_back(nom);
        }

        for (size_t k = 0; k < cites.size(); ++k)
                for (size_t j = 0; j < vus.size(); ++j)
                        if (vus[j].nom == cites[k])
                                vus[j].dit = true;

        // Tri : plus grand nombre d'abord, puis premiere observation, puis entree.
        for (size_t i = 1; i < vus.size(); ++i) {
                NkContournement clef = vus[i];
                size_t j = i;
                while (j > 0) {
                        const NkContournement &autre = vus[j - 1];
                        bool avant = false;
                        if (clef.fois != autre.fois)
                                avant = clef.fois > autre.fois;
                        else if (clef.premiere != autre.premiere)
                                avant = clef.premiere < autre.premiere;
                        else
                                avant = clef.rangEntree < autre.rangEntree;
                        if (!avant)
                                break;
                        vus[j] = autre;
                        --j;
                }
                vus[j] = clef;
        }

        long nonDits = 0;
        long foisNonDites = 0, foisTotales = 0;
        for (size_t i = 0; i < vus.size(); ++i) {
                std::printf("%s %ld %s\n", vus[i].nom.c_str(), vus[i].fois,
                            vus[i].dit ? "DIT" : "NON_DIT");
                foisTotales += vus[i].fois;
                if (!vus[i].dit) {
                        ++nonDits;
                        foisNonDites += vus[i].fois;
                }
        }

        const long part = foisTotales > 0 ? (100 * foisNonDites) / foisTotales : 0;
        std::printf("NON_DITS %ld\n", nonDits);
        std::printf("PART_NON_DITE %ld\n", part);

        std::printf("DIT_SANS_VU");
        bool premier = true;
        for (size_t i = 0; i < cites.size(); ++i) {
                bool vu = false;
                for (size_t j = 0; j < vus.size(); ++j)
                        if (vus[j].nom == cites[i])
                                vu = true;
                if (!vu) {
                        std::printf(" %s", cites[i].c_str());
                        premier = false;
                }
        }
        if (premier)
                std::printf(" AUCUN");
        std::printf("\n");
        std::printf("HORS_SEANCE %ld\n", horsSeance);
        return 0;
}
