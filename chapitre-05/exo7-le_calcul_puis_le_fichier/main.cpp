// Exercice 7 - Le calcul, puis le fichier
// Calcule la taille non compressee de plusieurs sons, la compare a leur fichier,
// et decide lesquels se chargent en memoire et lesquels se lisent en flux.
#include <iostream>
#include <string>

using namespace std;

int main() {
    long long seuil = 0;
    int n = 0;
    cin >> seuil >> n;
    long long memoire = 0;
    int flux = 0, refuses = 0;
    for (int i = 0; i < n; ++i) {
        string nom;
        long long frequence = 0, canaux = 0, bits = 0, duree = 0, fichier = 0;
        cin >> nom >> frequence >> canaux >> bits >> duree >> fichier;
        // Seules les profondeurs 8, 16, 24 et 32 bits sont acceptees.
        if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {
            cout << nom << " REFUSE\n";
            ++refuses;
            continue;
        }
        // Les trois multiplications d'abord, la division par 1000 a la fin (64 bits).
        const long long brut = frequence * canaux * (bits / 8) * duree / 1000;
        const long long pourcent = fichier * 100 / brut;
        // Strictement plus grand que le seuil : flux. Egal au seuil : memoire.
        if (brut > seuil) {
            cout << nom << " " << brut << " " << pourcent << " FLUX\n";
            ++flux;
        } else {
            cout << nom << " " << brut << " " << pourcent << " MEMOIRE\n";
            memoire += brut;
        }
    }
    cout << "MEMOIRE " << memoire << "\n";
    cout << "FLUX " << flux << "\n";
    cout << "REFUSES " << refuses << "\n";
    return 0;
}
