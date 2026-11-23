// Range une liste de manques dans les quatre cases valeur/cout.
// Entree : N, puis N lignes « nom valeur cout etiquette genre [cible] ».
// Sortie : une ligne CASE nom etiquette par entree gardee, les lignes
// HORS_LISTE nom raison, puis VERDICT.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {

struct NkLigne {
        std::string nom;
        int valeur = 0;
        int cout = 0;
        int etiquette = 0; // 0 MESUREE, 1 OBSERVEE, 2 SUPPOSEE, 3 aucune
        int genre = 0;     // 0 MANQUE, 1 REECRITURE, 2 CHOIX, 3 SOCLE
        std::string cible;
        int rang = 0;
        int rangee = 0;
        bool gardee = false;
};

const char *EtiquetteDe(int etiquette) {
        if (etiquette == 0)
                return "MESUREE";
        if (etiquette == 1)
                return "OBSERVEE";
        return "SUPPOSEE";
}

const char *CaseDe(int rangee) {
        if (rangee == 0)
                return "MAINTENANT";
        if (rangee == 1)
                return "DECOUPER";
        if (rangee == 2)
                return "EN_PASSANT";
        return "ON_NE_LE_FAIT_PAS";
}

} // namespace

int main() {
        char mot[256];
        if (std::scanf("%255s", mot) != 1)
                return 0;
        long n = std::strtol(mot, nullptr, 10);
        if (n < 0)
                n = 0;

        std::vector<NkLigne> lignes;
        for (long i = 0; i < n; ++i) {
                char nom[256], etiquette[64], genre[64], cible[256];
                int valeur = 0, cout = 0;
                if (std::scanf("%255s %d %d %63s %63s", nom, &valeur, &cout, etiquette, genre) != 5)
                        break;

                NkLigne l;
                l.nom = nom;
                l.valeur = valeur;
                l.cout = cout;
                l.rang = static_cast<int>(i);
                if (std::strcmp(etiquette, "MESUREE") == 0)
                        l.etiquette = 0;
                else if (std::strcmp(etiquette, "OBSERVEE") == 0)
                        l.etiquette = 1;
                else if (std::strcmp(etiquette, "SUPPOSEE") == 0)
                        l.etiquette = 2;
                else
                        l.etiquette = 3;

                if (std::strcmp(genre, "MANQUE") == 0)
                        l.genre = 0;
                else if (std::strcmp(genre, "REECRITURE") == 0)
                        l.genre = 1;
                else if (std::strcmp(genre, "CHOIX") == 0)
                        l.genre = 2;
                else {
                        l.genre = 3;
                        if (std::scanf("%255s", cible) == 1)
                                l.cible = cible;
                }
                lignes.push_back(l);
        }

        std::vector<const NkLigne *> ecartees;
        std::vector<const char *> raisons;
        for (size_t i = 0; i < lignes.size(); ++i) {
                NkLigne &l = lignes[i];
                if (l.genre == 1) {
                        ecartees.push_back(&l);
                        raisons.push_back("reecriture");
                } else if (l.genre == 2) {
                        ecartees.push_back(&l);
                        raisons.push_back("choix");
                } else if (l.etiquette == 3) {
                        ecartees.push_back(&l);
                        raisons.push_back("sans_etiquette");
                } else {
                        l.gardee = true;
                }
        }

        // Un socle n'apporte rien seul : il prend la valeur de sa cible quand
        // cette cible est un manque garde.
        for (size_t i = 0; i < lignes.size(); ++i) {
                NkLigne &l = lignes[i];
                if (!l.gardee || l.genre != 3)
                        continue;
                for (size_t j = 0; j < lignes.size(); ++j) {
                        const NkLigne &cible = lignes[j];
                        if (cible.nom == l.cible && cible.gardee && cible.genre == 0) {
                                if (cible.valeur > l.valeur)
                                        l.valeur = cible.valeur;
                                break;
                        }
                }
        }

        for (size_t i = 0; i < lignes.size(); ++i) {
                NkLigne &l = lignes[i];
                if (!l.gardee)
                        continue;
                const bool forte = l.valeur >= 3;
                const bool eleve = l.cout >= 3;
                if (forte && !eleve)
                        l.rangee = 0;
                else if (forte && eleve)
                        l.rangee = 1;
                else if (!forte && !eleve)
                        l.rangee = 2;
                else
                        l.rangee = 3;
        }

        std::vector<const NkLigne *> ordre;
        for (size_t i = 0; i < lignes.size(); ++i)
                if (lignes[i].gardee)
                        ordre.push_back(&lignes[i]);

        for (size_t i = 1; i < ordre.size(); ++i) {
                const NkLigne *clef = ordre[i];
                size_t j = i;
                while (j > 0) {
                        const NkLigne *autre = ordre[j - 1];
                        bool avant = false;
                        if (clef->rangee != autre->rangee)
                                avant = clef->rangee < autre->rangee;
                        else if (clef->etiquette != autre->etiquette)
                                avant = clef->etiquette < autre->etiquette;
                        else if (clef->valeur != autre->valeur)
                                avant = clef->valeur > autre->valeur;
                        else if (clef->cout != autre->cout)
                                avant = clef->cout < autre->cout;
                        else
                                avant = clef->rang < autre->rang;
                        if (!avant)
                                break;
                        ordre[j] = autre;
                        --j;
                }
                ordre[j] = clef;
        }

        bool refuse = false;
        for (size_t i = 0; i < ordre.size(); ++i) {
                std::printf("%s %s %s\n", CaseDe(ordre[i]->rangee), ordre[i]->nom.c_str(),
                            EtiquetteDe(ordre[i]->etiquette));
                if (ordre[i]->rangee == 3)
                        refuse = true;
        }
        for (size_t i = 0; i < ecartees.size(); ++i)
                std::printf("HORS_LISTE %s %s\n", ecartees[i]->nom.c_str(), raisons[i]);

        std::printf("VERDICT %s\n", refuse ? "HONNETE" : "COMPLAISANT");
        return 0;
}
