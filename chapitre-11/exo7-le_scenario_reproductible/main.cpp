#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

const int kMaxElements = 8;

struct NkAction {
        enum Genre { Pan, Zoom, Depot, Clic, Move, Pack, Grille, Capture } genre;
        long tour = 0;
        long champ[3] = {0, 0, 0};
        int nchamps = 0;
        std::string texte;
};

struct NkDepot {
        long tour = 0;
        long px = 0;
        long py = 0;
        std::string chemin;
};

struct NkClic {
        long tour = 0;
        long px = 0;
        long py = 0;
};

bool Entier(const std::string &texte, long &valeur) {
        if (texte.empty())
                return false;
        size_t i = 0;
        if (texte[0] == '-' || texte[0] == '+')
                ++i;
        if (i == texte.size())
                return false;
        for (size_t k = i; k < texte.size(); ++k)
                if (texte[k] < '0' || texte[k] > '9')
                        return false;
        valeur = std::strtol(texte.c_str(), nullptr, 10);
        return true;
}

// Coupe sur un separateur, en gardant les morceaux vides.
void Couper(const std::string &valeur, char separateur, std::vector<std::string> &morceaux) {
        morceaux.clear();
        size_t debut = 0;
        while (debut <= valeur.size()) {
                const size_t fin = valeur.find(separateur, debut);
                if (fin == std::string::npos) {
                        morceaux.push_back(valeur.substr(debut));
                        return;
                }
                morceaux.push_back(valeur.substr(debut, fin - debut));
                debut = fin + 1;
        }
}

} // namespace

int main() {
        std::vector<NkAction> actions;
        std::vector<NkDepot> depots;
        std::vector<NkClic> clics;
        std::vector<long> captures;
        long sortie = 0;
        long ignores = 0;

        char tampon[1 << 16];
        while (std::fgets(tampon, sizeof(tampon), stdin) != nullptr) {
                std::string ligne = tampon;
                while (!ligne.empty() && (ligne.back() == '\n' || ligne.back() == '\r'))
                        ligne.pop_back();
                const size_t egal = ligne.find('=');
                if (egal == std::string::npos)
                        continue;
                const std::string nom = ligne.substr(0, egal);
                const std::string valeur = ligne.substr(egal + 1);
                std::vector<std::string> morceaux;

                if (nom == "NK_AGENT_EXIT") {
                        long n = 0;
                        if (!Entier(valeur, n))
                                ++ignores;
                        else
                                sortie = n;
                        continue;
                }

                if (nom == "NK_AGENT_PAN" || nom == "NK_AGENT_MOVE" || nom == "NK_AGENT_ZOOM" ||
                    nom == "NK_AGENT_GRID") {
                        const size_t minimum =
                                nom == "NK_AGENT_ZOOM" ? 4 : (nom == "NK_AGENT_GRID" ? 2 : 3);
                        Couper(valeur, ':', morceaux);
                        bool valide = morceaux.size() >= minimum;
                        long nombres[4] = {0, 0, 0, 0};
                        // Le dernier champ de GRID (affichee ou cachee) se
                        // recopie tel quel ; tous les autres sont des nombres.
                        const size_t aLire = nom == "NK_AGENT_GRID" ? 1 : minimum;
                        for (size_t i = 0; valide && i < aLire; ++i)
                                valide = Entier(morceaux[i], nombres[i]);
                        if (!valide) {
                                ++ignores;
                                continue;
                        }
                        NkAction a;
                        a.genre = nom == "NK_AGENT_PAN"    ? NkAction::Pan
                                  : nom == "NK_AGENT_MOVE" ? NkAction::Move
                                  : nom == "NK_AGENT_ZOOM" ? NkAction::Zoom
                                                           : NkAction::Grille;
                        a.tour = nombres[0];
                        a.nchamps = static_cast<int>(minimum) - 1;
                        a.champ[0] = nombres[1];
                        a.champ[1] = nombres[2];
                        a.champ[2] = nombres[3];
                        if (nom == "NK_AGENT_GRID")
                                a.texte = morceaux[1];
                        actions.push_back(a);
                        continue;
                }

                if (nom == "NK_AGENT_PACK") {
                        long n = 0;
                        if (!Entier(valeur, n)) {
                                ++ignores;
                                continue;
                        }
                        NkAction a;
                        a.genre = NkAction::Pack;
                        a.tour = n;
                        actions.push_back(a);
                        continue;
                }

                if (nom == "NK_AGENT_SHOT") {
                        Couper(valeur, ',', morceaux);
                        int gardes = 0;
                        for (size_t i = 0; i < morceaux.size(); ++i) {
                                long n = 0;
                                if (!Entier(morceaux[i], n) || gardes == kMaxElements) {
                                        ++ignores;
                                        continue;
                                }
                                captures.push_back(n);
                                ++gardes;
                        }
                        continue;
                }

                if (nom == "NK_AGENT_CLICK" || nom == "NK_AGENT_DROP") {
                        Couper(valeur, ';', morceaux);
                        int gardes = 0;
                        for (size_t i = 0; i < morceaux.size(); ++i) {
                                const std::string &element = morceaux[i];
                                long tour = 0, px = 0, py = 0;
                                bool valide = false;
                                std::string chemin;
                                if (nom == "NK_AGENT_CLICK") {
                                        std::vector<std::string> champs;
                                        Couper(element, ':', champs);
                                        valide = champs.size() >= 3 && Entier(champs[0], tour) &&
                                                 Entier(champs[1], px) && Entier(champs[2], py);
                                } else {
                                        // Le chemin est tout ce qui suit le troisieme deux-points.
                                        const size_t p1 = element.find(':');
                                        const size_t p2 = p1 == std::string::npos ? p1 : element.find(':', p1 + 1);
                                        const size_t p3 = p2 == std::string::npos ? p2 : element.find(':', p2 + 1);
                                        if (p3 != std::string::npos && Entier(element.substr(0, p1), tour) &&
                                            Entier(element.substr(p1 + 1, p2 - p1 - 1), px) &&
                                            Entier(element.substr(p2 + 1, p3 - p2 - 1), py)) {
                                                chemin = element.substr(p3 + 1);
                                                valide = true;
                                        }
                                }
                                if (!valide || gardes == kMaxElements) {
                                        ++ignores;
                                        continue;
                                }
                                if (nom == "NK_AGENT_CLICK")
                                        clics.push_back(NkClic{tour, px, py});
                                else
                                        depots.push_back(NkDepot{tour, px, py, chemin});
                                ++gardes;
                        }
                        continue;
                }
        }

        // Dernier tour utile quand rien n'arrete la boucle : au-dela, plus rien
        // ne peut arriver.
        long dernierTour = 0;
        for (size_t i = 0; i < actions.size(); ++i)
                if (actions[i].tour > dernierTour)
                        dernierTour = actions[i].tour;
        for (size_t i = 0; i < depots.size(); ++i)
                if (depots[i].tour > dernierTour)
                        dernierTour = depots[i].tour;
        for (size_t i = 0; i < clics.size(); ++i)
                if (clics[i].tour > dernierTour)
                        dernierTour = clics[i].tour;
        for (size_t i = 0; i < captures.size(); ++i)
                if (captures[i] > dernierTour)
                        dernierTour = captures[i];
        if (dernierTour < 0)
                dernierTour = 0;

        std::vector<char> actionFaite(actions.size(), 0);
        std::vector<char> depotFait(depots.size(), 0);
        std::vector<char> clicFait(clics.size(), 0);
        std::vector<char> captureFaite(captures.size(), 0);

        long tour = 0;
        bool arret = false;
        for (;;) {
                if (tour > 0) {
                        // Ordre impose dans le tour : PAN, ZOOM, les depots, les
                        // clics, MOVE, le rangement, la grille.
                        for (size_t i = 0; i < actions.size(); ++i)
                                if (!actionFaite[i] && actions[i].genre == NkAction::Pan &&
                                    actions[i].tour == tour) {
                                        actionFaite[i] = 1;
                                        std::printf("TOUR %ld PAN %ld %ld\n", tour, actions[i].champ[0],
                                                    actions[i].champ[1]);
                                }
                        for (size_t i = 0; i < actions.size(); ++i)
                                if (!actionFaite[i] && actions[i].genre == NkAction::Zoom &&
                                    actions[i].tour == tour) {
                                        actionFaite[i] = 1;
                                        std::printf("TOUR %ld ZOOM %ld %ld %ld\n", tour, actions[i].champ[0],
                                                    actions[i].champ[1], actions[i].champ[2]);
                                }
                        for (size_t i = 0; i < depots.size(); ++i)
                                if (!depotFait[i] && depots[i].tour == tour) {
                                        depotFait[i] = 1;
                                        std::printf("TOUR %ld DEPOT %s %ld %ld\n", tour, depots[i].chemin.c_str(),
                                                    depots[i].px, depots[i].py);
                                }
                        for (size_t i = 0; i < clics.size(); ++i)
                                if (!clicFait[i] && clics[i].tour == tour) {
                                        clicFait[i] = 1;
                                        std::printf("TOUR %ld CLIC %ld %ld\n", tour, clics[i].px, clics[i].py);
                                }
                        for (size_t i = 0; i < actions.size(); ++i)
                                if (!actionFaite[i] && actions[i].genre == NkAction::Move &&
                                    actions[i].tour == tour) {
                                        actionFaite[i] = 1;
                                        std::printf("TOUR %ld MOVE %ld %ld\n", tour, actions[i].champ[0],
                                                    actions[i].champ[1]);
                                }
                        for (size_t i = 0; i < actions.size(); ++i)
                                if (!actionFaite[i] && actions[i].genre == NkAction::Pack &&
                                    actions[i].tour == tour) {
                                        actionFaite[i] = 1;
                                        std::printf("TOUR %ld RANGE\n", tour);
                                }
                        for (size_t i = 0; i < actions.size(); ++i)
                                if (!actionFaite[i] && actions[i].genre == NkAction::Grille &&
                                    actions[i].tour == tour) {
                                        actionFaite[i] = 1;
                                        std::printf("TOUR %ld GRILLE %s\n", tour, actions[i].texte.c_str());
                                }
                }

                // Le compteur passe a tour + 1 : ce sont ces captures qui ont lieu.
                const long suivant = tour + 1;
                for (size_t i = 0; i < captures.size(); ++i)
                        if (!captureFaite[i] && captures[i] == suivant) {
                                captureFaite[i] = 1;
                                std::printf("TOUR %ld CAPTURE\n", tour);
                        }

                if (sortie > 0 && suivant >= sortie) {
                        arret = true;
                        break;
                }
                if (sortie <= 0 && suivant > dernierTour)
                        break;
                ++tour;
        }

        if (arret)
                std::printf("ARRET %ld\n", tour);
        else
                std::printf("SANS_ARRET\n");

        long jamais = 0;
        for (size_t i = 0; i < actionFaite.size(); ++i)
                if (!actionFaite[i])
                        ++jamais;
        for (size_t i = 0; i < depotFait.size(); ++i)
                if (!depotFait[i])
                        ++jamais;
        for (size_t i = 0; i < clicFait.size(); ++i)
                if (!clicFait[i])
                        ++jamais;
        for (size_t i = 0; i < captureFaite.size(); ++i)
                if (!captureFaite[i])
                        ++jamais;

        std::printf("JAMAIS %ld\n", jamais);
        std::printf("IGNORES %ld\n", ignores);
        return 0;
}
