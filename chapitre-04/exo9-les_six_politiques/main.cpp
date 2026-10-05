#include <iostream>
#include <string>

namespace {

struct Resultat {
    std::string nom;
    long long vx = 0;
    long long vy = 0;
    long long vw = 0;
    long long vh = 0;
    long long mw = 0;
    long long mh = 0;
};

long long arrondi(long long a, long long b) {
    return (2 * a + b) / (2 * b);
}

Resultat suivreFenetre(const std::string &nom, long long w, long long h) {
    Resultat r;
    r.nom = nom;
    r.vw = w;
    r.vh = h;
    r.mw = w;
    r.mh = h;
    return r;
}

Resultat letterbox(const std::string &nom, long long w, long long h, long long rw, long long rh) {
    Resultat r;
    r.nom = nom;
    if (w * rh <= h * rw) {
        r.vw = w;
        r.vh = arrondi(rh * w, rw);
    } else {
        r.vh = h;
        r.vw = arrondi(rw * h, rh);
    }
    r.vx = (w - r.vw) / 2;
    r.vy = (h - r.vh) / 2;
    r.mw = rw;
    r.mh = rh;
    return r;
}

}  // namespace

int main() {
    long long rw = 0, rh = 0, aw = 0, ah = 0, w = 1, h = 1;
    std::cin >> rw >> rh >> aw >> ah >> w >> h;

    const bool reference = (rw > 0 && rh > 0);

    Resultat liste[6];

    liste[0] = suivreFenetre("FOLLOW_WINDOW", w, h);

    if (reference) {
        Resultat r;
        r.nom = "STRETCH";
        r.vw = w;
        r.vh = h;
        r.mw = rw;
        r.mh = rh;
        liste[1] = r;
    } else {
        liste[1] = suivreFenetre("STRETCH", w, h);
    }

    if (reference) {
        liste[2] = letterbox("FIT_LETTERBOX", w, h, rw, rh);
    } else {
        liste[2] = suivreFenetre("FIT_LETTERBOX", w, h);
    }

    if (reference) {
        if (w >= rw && h >= rh) {
            const long long kw = w / rw;
            const long long kh = h / rh;
            const long long k = (kw < kh) ? kw : kh;
            Resultat r;
            r.nom = "INTEGER_SCALE";
            r.vw = rw * k;
            r.vh = rh * k;
            r.vx = (w - r.vw) / 2;
            r.vy = (h - r.vh) / 2;
            r.mw = rw;
            r.mh = rh;
            liste[3] = r;
        } else {
            liste[3] = letterbox("INTEGER_SCALE", w, h, rw, rh);
        }
    } else {
        liste[3] = suivreFenetre("INTEGER_SCALE", w, h);
    }

    if (reference) {
        Resultat r;
        r.nom = "FIT_CROP";
        r.vw = w;
        r.vh = h;
        if (w * rh > h * rw) {
            r.mw = rw;
            r.mh = arrondi(rw * h, w);
        } else {
            r.mw = arrondi(rh * w, h);
            r.mh = rh;
        }
        liste[4] = r;
    } else {
        liste[4] = suivreFenetre("FIT_CROP", w, h);
    }

    {
        Resultat r;
        r.nom = "MANUAL";
        r.vw = aw;
        r.vh = ah;
        r.mw = aw;
        r.mh = ah;
        liste[5] = r;
    }

    int bandes = 0;
    for (const Resultat &r : liste) {
        std::cout << r.nom << ' ' << r.vx << ' ' << r.vy << ' ' << r.vw << ' ' << r.vh << ' '
                  << r.mw << ' ' << r.mh << '\n';
        if (r.vw < w || r.vh < h) {
            ++bandes;
        }
    }

    const bool deformation = reference && (w * rh != h * rw);

    std::cout << "BANDES " << bandes << '\n';
    std::cout << "DEFORMATION " << (deformation ? "OUI" : "NON") << '\n';
    return 0;
}
