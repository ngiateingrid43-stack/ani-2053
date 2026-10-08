// Exercice 8 - Un son, vingt voix
// Rejoue une rafale de sons de trois manieres : un echantillon et une voix par lecture,
// un fichier recharge a chaque fois, une seule voix rejouee.
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    long long D = 0, C = 0;
    int n = 0;
    cin >> D >> C >> n;
    vector<long long> t(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) cin >> t[static_cast<size_t>(i)];

    long long voixMax = 0, retardMax = 0;
    int coupes = 0;
    long long finChargement = 0; // fin du chargement precedent
    int premiereActive = 0;      // premiere lecture qui n'a pas fini a l'instant courant

    for (int i = 0; i < n; ++i) {
        const size_t k = static_cast<size_t>(i);
        // 1. Une voix par lecture : la lecture j joue de t_j a t_j + D, fin exclue.
        // Les instants sont croissants, donc les fins aussi : un seul pointeur avance.
        while (premiereActive < i && t[static_cast<size_t>(premiereActive)] + D <= t[k]) ++premiereActive;
        const long long voix = i - premiereActive + 1;

        // 2. Les chargements se font l'un apres l'autre et chacun bloque le programme.
        const long long debut = (i == 0) ? t[k] : max(t[k], finChargement);
        finChargement = debut + C;
        // 3. Le retard est la fin du chargement moins l'instant demande.
        const long long retard = finChargement - t[k];

        // 4. Une seule voix, rejouee : coupee si la suivante commence avant t_i + D.
        const bool coupe = (i + 1 < n) && (t[k + 1] < t[k] + D);

        cout << t[k] << " " << voix << " " << retard << " " << (coupe ? "COUPE" : "ENTIER") << "\n";
        voixMax = max(voixMax, voix);
        retardMax = max(retardMax, retard);
        if (coupe) ++coupes;
    }
    cout << "VOIX_MAX " << voixMax << "\n";
    cout << "RETARD_MAX " << retardMax << "\n";
    cout << "COUPES " << coupes << "\n";
    return 0;
}
