// Exercice 6 - La ligne de base
// Pose un texte glyphe par glyphe sur sa ligne de base.
// Dit ce qui monte, ce qui descend, et si le texte sort de l'ecran.
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

using namespace std;

struct Glyphe {
    long long avance, x0, y0, x1, y1;
};

int main() {
    int G = 0;
    cin >> G;
    map<char, Glyphe> table;
    for (int i = 0; i < G; ++i) {
        string c;
        Glyphe g{};
        cin >> c >> g.avance >> g.x0 >> g.y0 >> g.x1 >> g.y1;
        table[c[0]] = g;
    }
    int K = 0;
    cin >> K;
    map<string, long long> crenage;
    for (int i = 0; i < K; ++i) {
        string ab;
        long long k = 0;
        cin >> ab >> k;
        crenage[ab] = k;
    }
    string texte;
    long long ox = 0, oy = 0;
    cin >> texte >> ox >> oy;

    long long x = ox;
    bool dessine = false;
    long long minx = 0, miny = 0, maxx = 0, maxy = 0;
    int absents = 0;

    for (size_t i = 0; i < texte.size(); ++i) {
        const char c = texte[i];
        auto it = table.find(c);
        if (it == table.end()) {
            // Absent : il n'avance pas le curseur, et aucun crenage ne se cherche apres lui.
            cout << c << " ABSENT\n";
            ++absents;
            continue;
        }
        const Glyphe &g = it->second;
        cout << c << " " << x << "\n";
        // Le caractere est dessine si son rectangle n'est pas vide.
        if (g.x1 > g.x0 && g.y1 > g.y0) {
            const long long rx0 = x + g.x0, ry0 = oy + g.y0, rx1 = x + g.x1, ry1 = oy + g.y1;
            if (!dessine) {
                minx = rx0;
                miny = ry0;
                maxx = rx1;
                maxy = ry1;
                dessine = true;
            } else {
                minx = min(minx, rx0);
                miny = min(miny, ry0);
                maxx = max(maxx, rx1);
                maxy = max(maxy, ry1);
            }
        }
        x += g.avance;
        // Crenage avec le caractere suivant du texte, s'il forme un couple de la table.
        if (i + 1 < texte.size()) {
            string couple;
            couple += c;
            couple += texte[i + 1];
            auto kt = crenage.find(couple);
            if (kt != crenage.end()) x += kt->second;
        }
    }

    cout << "CURSEUR " << x << "\n";
    if (!dessine) {
        cout << "BOITE AUCUNE\n";
        cout << "MONTE 0\n";
        cout << "DESCEND 0\n";
        cout << "ECRAN RIEN\n";
    } else {
        cout << "BOITE " << minx << " " << miny << " " << maxx << " " << maxy << "\n";
        cout << "MONTE " << (miny < oy ? oy - miny : 0) << "\n";
        cout << "DESCEND " << (maxy > oy ? maxy - oy : 0) << "\n";
        // Le haut de l'ecran est y = 0.
        if (maxy <= 0) cout << "ECRAN HORS\n";
        else if (miny < 0) cout << "ECRAN COUPE\n";
        else cout << "ECRAN VISIBLE\n";
    }
    cout << "ABSENTS " << absents << "\n";
    return 0;
}
