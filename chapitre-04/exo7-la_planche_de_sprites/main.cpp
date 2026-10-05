#include <iostream>

int main() {
    long long colonnes = 0;
    long long lignes = 0;
    long long w = 0;
    long long h = 0;
    long long nbCases = 1;
    long long duree = 1;
    long long plafond = 1;
    std::cin >> colonnes >> lignes >> w >> h >> nbCases >> duree >> plafond;

    int n = 0;
    std::cin >> n;

    long long courante = 0;
    long long accumule = 0;
    long long avances = 0;
    long long plafonnes = 0;

    for (int i = 0; i < n; ++i) {
        long long dt = 0;
        if (!(std::cin >> dt)) {
            break;
        }

        if (dt > plafond) {
            dt = plafond;
            ++plafonnes;
        }
        accumule += dt;

        const long long pas = accumule / duree;
        accumule -= pas * duree;
        courante = (courante + pas) % nbCases;
        avances += pas;

        std::cout << courante << ' ' << (courante % colonnes) * w << ' ' << (courante / colonnes) * h << ' '
                  << w << ' ' << h << '\n';
    }

    std::cout << "AVANCES " << avances << '\n';
    std::cout << "PLAFONNES " << plafonnes << '\n';
    return 0;
}
