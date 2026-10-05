#include <iostream>
#include <string>

int main() {
    long long v = 0;
    int n = 0;
    std::cin >> v >> n;

    bool espace = false;
    bool gauche = false;
    bool droite = false;

    long long xe = 0;
    long long xi = 0;
    long long sautsEvenements = 0;
    long long sautsInterrogation = 0;
    long long manques = 0;

    for (int i = 1; i <= n; ++i) {
        int k = 0;
        if (!(std::cin >> k)) {
            break;
        }

        long long appuisEspace = 0;

        for (int j = 0; j < k; ++j) {
            std::string evt;
            std::cin >> evt;
            if (evt.size() < 2) {
                continue;
            }
            const bool enfonce = (evt[0] == '+');
            const std::string nom = evt.substr(1);

            if (nom == "SPACE") {
                espace = enfonce;
                if (enfonce) {
                    ++sautsEvenements;
                    ++appuisEspace;
                }
            } else if (nom == "RIGHT") {
                droite = enfonce;
                if (enfonce) {
                    xe += v;
                }
            } else if (nom == "LEFT") {
                gauche = enfonce;
                if (enfonce) {
                    xe -= v;
                }
            }
        }

        if (espace) {
            ++sautsInterrogation;
        } else {
            manques += appuisEspace;
        }
        if (droite) {
            xi += v;
        }
        if (gauche) {
            xi -= v;
        }

        std::cout << i << ' ' << xe << ' ' << xi << '\n';
    }

    std::cout << "SAUTS EVENEMENTS " << sautsEvenements << '\n';
    std::cout << "SAUTS INTERROGATION " << sautsInterrogation << '\n';
    std::cout << "MANQUES " << manques << '\n';
    return 0;
}
