// Exercice 5 - Trois polices, un atlas
// Range les glyphes de plusieurs polices dans une seule texture, par etageres,
// et agrandit la texture tant qu'ils n'y tiennent pas.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Groupe {
    string police;
    long long n, w, h;
};

struct Place {
    long long x1, y1, x2, y2;
};

// Un essai range tous les glyphes, groupe apres groupe, dans l'ordre de lecture.
static bool essai(const vector<Groupe> &groupes, long long P, long long W, long long H,
                  vector<Place> &places) {
    places.assign(groupes.size(), Place{0, 0, 0, 0});
    long long x = P, y = P, e = 0;
    for (size_t g = 0; g < groupes.size(); ++g) {
        for (long long k = 0; k < groupes[g].n; ++k) {
            const long long rw = groupes[g].w + P;
            const long long rh = groupes[g].h + P;
            if (x + rw > W - P) {
                // etagere suivante : la hauteur e compte deja la marge, et on ajoute P encore
                x = P;
                y = y + e + P;
                e = 0;
                if (x + rw > W - P) return false;
            }
            if (y + rh > H - P) return false;
            if (k == 0) {
                places[g].x1 = x;
                places[g].y1 = y;
            }
            places[g].x2 = x;
            places[g].y2 = y;
            x += rw;
            e = max(e, rh);
        }
    }
    return true;
}

int main() {
    long long P = 0, L = 0;
    int G = 0;
    cin >> P >> L >> G;
    vector<Groupe> groupes;
    for (int i = 0; i < G; ++i) {
        Groupe g;
        cin >> g.police >> g.n >> g.w >> g.h;
        groupes.push_back(g);
    }
    if (groupes.empty()) {
        cout << "AUCUN\n";
        return 0;
    }

    // Largeur de depart.
    long long W = L;
    if (L == 0) {
        W = 512;
        long long besoin = 0;
        for (const Groupe &g : groupes) besoin += g.n * (g.w + P) * (g.h + P);
        while (W * W < 2 * besoin && W < 4096) W *= 2;
    }
    long long H = W;

    vector<Place> places;
    int reussi = 0;
    for (int tentative = 1; tentative <= 8; ++tentative) {
        if (essai(groupes, P, W, H, places)) {
            reussi = tentative;
            break;
        }
        // Apres un essai manque, la texture grandit.
        if (W == H) W *= 2;
        else H = W;
    }

    if (reussi == 0) {
        cout << "ESSAIS 8\n";
        cout << "ECHEC\n";
        return 0;
    }

    long long occupe = 0;
    for (const Groupe &g : groupes) occupe += g.n * g.w * g.h;
    const long long surface = W * H;
    const long long perdu = (surface - occupe) * 100 / surface;

    cout << "ESSAIS " << reussi << "\n";
    cout << "TEXTURE " << W << " " << H << "\n";
    for (size_t g = 0; g < groupes.size(); ++g)
        cout << groupes[g].police << " " << places[g].x1 << " " << places[g].y1 << " "
             << places[g].x2 << " " << places[g].y2 << "\n";
    cout << "OCCUPE " << occupe << "\n";
    cout << "PERDU " << perdu << "\n";
    return 0;
}
