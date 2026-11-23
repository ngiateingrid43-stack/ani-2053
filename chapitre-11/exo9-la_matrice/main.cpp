#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

struct NkEntree {
        std::string nom;
        int valeur = 0;
        int cout = 0;
        int etiquette = 0; // 0 MESUREE, 1 OBSERVEE, 2 SUPPOSEE, 3 aucune
        int genre = 0;     // 0 MANQUE, 1 REECRITURE, 2 CHOIX, 3 SOCLE
        std::string cible;
        int rang = 0;
        int caseFinale = 0;
        bool gardee = false;
};

const char *NomEtiquette(int etiquette) {
        switch (etiquette) {
        case 0:
                return "MESUREE";
        case 1:
                return "OBSERVEE";
        default:
                return "SUPPOSEE";
        }
}

const char *NomCase(int c) {
        switch (c) {
        case 0:
                return "MAINTENANT";
        case 1:
                return "DECOUPER";
        case 2:
                return "EN_PASSANT";
        default:
                return "ON_NE_LE_FAIT_PAS";
        }
}

} // namespace

int main() {
        char mot[256];
        long n = 0;
        if (std::scanf("%255s", mot) != 1)
                return 0;
        n = std::strtol(mot, nullptr, 10);
        if (n < 0)
                n = 0;

        std::vector<NkEntree> entrees;
        for (long i = 0; i < n; ++i) {
                NkEntree e;
                char nom[256], etiquette[64], genre[64], cible[256];
                int valeur = 0, cout = 0;
                if (std::scanf("%255s %d %d %63s %63s", nom, &valeur, &cout, etiquette, genre) != 5)
                        break;
                e.nom = nom;
                e.valeur = valeur;
                e.cout = cout;
                e.rang = static_cast<int>(i);
                if (std::strcmp(etiquette, "MESUREE") == 0)
                        e.etiquette = 0;
                else if (std::strcmp(etiquette, "OBSERVEE") == 0)
                        e.etiquette = 1;
                else if (std::strcmp(etiquette, "SUPPOSEE") == 0)
                        e.etiquette = 2;
                else
                        e.etiquette = 3;

                if (std::strcmp(genre, "MANQUE") == 0)
                        e.genre = 0;
                else if (std::strcmp(genre, "REECRITURE") == 0)
                        e.genre = 1;
                else if (std::strcmp(genre, "CHOIX") == 0)
                        e.genre = 2;
                else {
                        e.genre = 3;
                        if (std::scanf("%255s", cible) == 1)
                                e.cible = cible;
                }
                entrees.push_back(e);
        }

        // Ce qui est ecarte, releve dans l'ordre de l'entree et affiche apres
        // les lignes CASE.
        std::vector<const NkEntree *> ecartees;
        std::vector<const char *> raisons;
        for (size_t i = 0; i < entrees.size(); ++i) {
                NkEntree &e = entrees[i];
                if (e.genre == 1) {
                        ecartees.push_back(&e);
                        raisons.push_back("reecriture");
                } else if (e.genre == 2) {
                        ecartees.push_back(&e);
                        raisons.push_back("choix");
                } else if (e.etiquette == 3) {
                        ecartees.push_back(&e);
                        raisons.push_back("sans_etiquette");
                } else {
                        e.gardee = true;
                }
        }

        // La valeur d'un socle : la plus grande de la sienne et de celle de sa
        // cible, quand cette cible est un MANQUE garde.
        for (size_t i = 0; i < entrees.size(); ++i) {
                NkEntree &e = entrees[i];
                if (!e.gardee || e.genre != 3)
                        continue;
                for (size_t j = 0; j < entrees.size(); ++j) {
                        const NkEntree &cible = entrees[j];
                        if (cible.nom == e.cible && cible.gardee && cible.genre == 0) {
                                if (cible.valeur > e.valeur)
                                        e.valeur = cible.valeur;
                                break;
                        }
                }
        }

        for (size_t i = 0; i < entrees.size(); ++i) {
                NkEntree &e = entrees[i];
                if (!e.gardee)
                        continue;
                const bool forte = e.valeur >= 3;
                const bool chevre = e.cout >= 3;
                if (forte && !chevre)
                        e.caseFinale = 0;
                else if (forte && chevre)
                        e.caseFinale = 1;
                else if (!forte && !chevre)
                        e.caseFinale = 2;
                else
                        e.caseFinale = 3;
        }

        // Tri : case, etiquette, valeur decroissante, cout croissant, entree.
        std::vector<const NkEntree *> rangees;
        for (size_t i = 0; i < entrees.size(); ++i)
                if (entrees[i].gardee)
                        rangees.push_back(&entrees[i]);

        for (size_t i = 1; i < rangees.size(); ++i) {
                const NkEntree *clef = rangees[i];
                size_t j = i;
                while (j > 0) {
                        const NkEntree *autre = rangees[j - 1];
                        bool avant = false;
                        if (clef->caseFinale != autre->caseFinale)
                                avant = clef->caseFinale < autre->caseFinale;
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
                        rangees[j] = autre;
                        --j;
                }
                rangees[j] = clef;
        }

        bool quelqueChoseRefuse = false;
        for (size_t i = 0; i < rangees.size(); ++i) {
                std::printf("%s %s %s\n", NomCase(rangees[i]->caseFinale), rangees[i]->nom.c_str(),
                            NomEtiquette(rangees[i]->etiquette));
                if (rangees[i]->caseFinale == 3)
                        quelqueChoseRefuse = true;
        }

        for (size_t i = 0; i < ecartees.size(); ++i)
                std::printf("HORS_LISTE %s %s\n", ecartees[i]->nom.c_str(), raisons[i]);

        std::printf("VERDICT %s\n", quelqueChoseRefuse ? "HONNETE" : "COMPLAISANT");
        return 0;
}
