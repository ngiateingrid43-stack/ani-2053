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

        // Une operation par ligne : on decoupe en lignes, puis en mots.
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

bool Egal(const char *a, const char *b) { return std::strcmp(a, b) == 0; }

/// Une operation telle qu'elle s'ecrit : son verbe et, s'il y en a un, son nom.
struct NkOperation {
        enum Genre { Depose, Deplace, Supprime, Miroir, Range, Trait, Efface } genre;
        const char *nom;
        int numeroDeTrait;
};

void EcrireOperation(const NkOperation &op) {
        switch (op.genre) {
        case NkOperation::Depose:
                std::printf("DEPOSE %s", op.nom);
                break;
        case NkOperation::Deplace:
                std::printf("DEPLACE %s", op.nom);
                break;
        case NkOperation::Supprime:
                std::printf("SUPPRIME %s", op.nom);
                break;
        case NkOperation::Miroir:
                std::printf("MIROIR %s", op.nom);
                break;
        case NkOperation::Range:
                std::printf("RANGE");
                break;
        case NkOperation::Trait:
                std::printf("TRAIT %d", op.numeroDeTrait);
                break;
        case NkOperation::Efface:
                std::printf("EFFACE");
                break;
        }
}

} // namespace

int main() {
        ChargerEntree();

        std::vector<NkOperation> attendues; // la pile de l'utilisateur
        std::vector<int> traitsPresents;    // la liste de l'outil
        int prochainTrait = 1;

        long justes = 0, riens = 0, trahis = 0;
        long annulables = 0, operations = 0;
        long numeroAnnulation = 0;

        for (;;) {
                const char *verbe = Jeton();
                if (*verbe == '\0')
                        break;

                if (Egal(verbe, "ANNULE")) {
                        // Ce que l'utilisateur attend : le sommet de sa pile.
                        const bool pileVide = attendues.empty();
                        NkOperation attendu;
                        attendu.genre = NkOperation::Efface;
                        attendu.nom = "";
                        attendu.numeroDeTrait = 0;
                        if (!pileVide) {
                                attendu = attendues.back();
                                attendues.pop_back();
                        }

                        // Ce que l'outil fait : il ne connait que les traits.
                        const bool traitReste = !traitsPresents.empty();
                        int traitDefait = 0;
                        if (traitReste) {
                                traitDefait = traitsPresents.back();
                                traitsPresents.pop_back();
                        }

                        ++numeroAnnulation;
                        std::printf("ANNULE %ld ATTENDU ", numeroAnnulation);
                        if (pileVide)
                                std::printf("AUCUNE");
                        else
                                EcrireOperation(attendu);

                        std::printf(" OBTENU ");
                        if (traitReste)
                                std::printf("TRAIT %d", traitDefait);
                        else
                                std::printf("RIEN");

                        const bool attenduTrait = !pileVide && attendu.genre == NkOperation::Trait;
                        bool juste = false;
                        if (traitReste && attenduTrait && traitDefait == attendu.numeroDeTrait)
                                juste = true;
                        if (!traitReste && pileVide)
                                juste = true;

                        if (juste) {
                                std::printf(" JUSTE\n");
                                ++justes;
                        } else if (!traitReste) {
                                std::printf(" RIEN\n");
                                ++riens;
                        } else {
                                std::printf(" TRAHI\n");
                                ++trahis;
                        }
                        continue;
                }

                NkOperation op;
                op.numeroDeTrait = 0;
                if (Egal(verbe, "DEPOSE"))
                        op.genre = NkOperation::Depose;
                else if (Egal(verbe, "DEPLACE"))
                        op.genre = NkOperation::Deplace;
                else if (Egal(verbe, "SUPPRIME"))
                        op.genre = NkOperation::Supprime;
                else if (Egal(verbe, "MIROIR"))
                        op.genre = NkOperation::Miroir;
                else if (Egal(verbe, "RANGE"))
                        op.genre = NkOperation::Range;
                else if (Egal(verbe, "TRAIT"))
                        op.genre = NkOperation::Trait;
                else if (Egal(verbe, "EFFACE"))
                        op.genre = NkOperation::Efface;
                else
                        continue; // verbe inconnu : rien n'entre dans la pile

                op.nom = "";
                if (op.genre == NkOperation::Depose || op.genre == NkOperation::Deplace ||
                    op.genre == NkOperation::Supprime || op.genre == NkOperation::Miroir)
                        op.nom = Jeton();

                if (op.genre == NkOperation::Trait) {
                        op.numeroDeTrait = prochainTrait++;
                        traitsPresents.push_back(op.numeroDeTrait);
                        ++annulables;
                } else if (op.genre == NkOperation::Efface) {
                        traitsPresents.clear();
                }

                ++operations;
                attendues.push_back(op);
        }

        std::printf("JUSTES %ld\n", justes);
        std::printf("RIENS %ld\n", riens);
        std::printf("TRAHIS %ld\n", trahis);
        std::printf("ANNULABLES %ld SUR %ld\n", annulables, operations);
        return 0;
}
