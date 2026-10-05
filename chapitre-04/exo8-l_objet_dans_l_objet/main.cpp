#include <iostream>
#include <map>
#include <string>

namespace {

long long ramener(long long angle) {
    return ((angle % 360) + 360) % 360;
}

struct Monde {
    long long x = 0;
    long long y = 0;
    long long angle = 0;
    long long echelle = 1;
    int niveau = 1;
};

}  // namespace

int main() {
    int n = 0;
    std::cin >> n;

    std::map<std::string, Monde> objets;
    int profondeur = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        std::string parent;
        long long tx = 0, ty = 0, angle = 0, echelle = 1;
        if (!(std::cin >> nom >> parent >> tx >> ty >> angle >> echelle)) {
            break;
        }

        Monde m;
        const auto it = objets.find(parent);
        if (parent == "-" || it == objets.end()) {
            m.x = tx;
            m.y = ty;
            m.angle = ramener(angle);
            m.echelle = echelle;
            m.niveau = 1;
        } else {
            const Monde &p = it->second;
            const long long ax = tx * p.echelle;
            const long long ay = ty * p.echelle;

            long long c = 1;
            long long s = 0;
            if (p.angle == 90) {
                c = 0;
                s = 1;
            } else if (p.angle == 180) {
                c = -1;
                s = 0;
            } else if (p.angle == 270) {
                c = 0;
                s = -1;
            }

            m.x = p.x + (ax * c - ay * s);
            m.y = p.y + (ax * s + ay * c);
            m.angle = ramener(p.angle + angle);
            m.echelle = p.echelle * echelle;
            m.niveau = p.niveau + 1;
        }

        if (m.niveau > profondeur) {
            profondeur = m.niveau;
        }
        objets[nom] = m;

        std::cout << nom << ' ' << m.x << ' ' << m.y << ' ' << m.angle << ' ' << m.echelle << '\n';
    }

    std::cout << "PROFONDEUR " << profondeur << '\n';
    return 0;
}
