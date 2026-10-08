// Exercice 3 - Les six formats
// Calcule la memoire d'une image dans deux formats de pixels,
// et ce qu'une conversion de l'un a l'autre lui fait perdre.
#include <iostream>
#include <string>

using namespace std;

struct Format {
    const char *nom;
    long long octetsParPixel;
    bool couleur;
    bool transparence;
    bool flottants;
};

static const Format kFormats[] = {
    {"GRAY8", 1, false, false, false},
    {"GRAY_A16", 2, false, true, false},
    {"RGB24", 3, true, false, false},
    {"RGBA32", 4, true, true, false},
    {"RGB96F", 12, true, false, true},
    {"RGBA128F", 16, true, true, true},
};

// Comparaison exacte, majuscules comprises.
static const Format *trouver(const string &nom) {
    for (const Format &f : kFormats)
        if (nom == f.nom) return &f;
    return nullptr;
}

int main() {
    long long w = 0, h = 0;
    int n = 0;
    cin >> w >> h >> n;
    const long long pixels = w * h; // 64 bits : une grande image deborde un int
    long long total = 0;
    int sansPerte = 0, refuses = 0;
    for (int i = 0; i < n; ++i) {
        string src, dst;
        cin >> src >> dst;
        const Format *s = trouver(src);
        const Format *d = trouver(dst);
        if (!s || !d) {
            cout << src << " " << dst << " REFUSE\n";
            ++refuses;
            continue;
        }
        const long long os = pixels * s->octetsParPixel;
        const long long od = pixels * d->octetsParPixel;
        // Les pertes se cherchent dans cet ordre.
        string pertes;
        auto ajouter = [&pertes](const char *p) {
            if (!pertes.empty()) pertes += "+";
            pertes += p;
        };
        if (s->transparence && !d->transparence) ajouter("TRANSPARENCE");
        if (s->couleur && !d->couleur) ajouter("COULEUR");
        if (s->flottants && !d->flottants) ajouter("ETENDUE");
        if (pertes.empty()) {
            pertes = "AUCUNE";
            ++sansPerte;
        }
        total += od;
        cout << src << " " << dst << " " << os << " " << od << " " << pertes << "\n";
    }
    cout << "TOTAL " << total << "\n";
    cout << "SANS_PERTE " << sansPerte << "\n";
    cout << "REFUSES " << refuses << "\n";
    return 0;
}
