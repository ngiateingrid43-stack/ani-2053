#include <algorithm>
#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    int refuses = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long w = 0, h = 0, px = 0, py = 0, ox = 0, oy = 0, sx = 0, sy = 0, angle = 0;
        if (!(std::cin >> nom >> w >> h >> px >> py >> ox >> oy >> sx >> sy >> angle)) {
            break;
        }

        if (angle % 90 != 0) {
            std::cout << nom << " ANGLE REFUSE\n";
            ++refuses;
            continue;
        }

        const long long a = ((angle % 360) + 360) % 360;
        long long c = 1;
        long long s = 0;
        if (a == 90) {
            c = 0;
            s = 1;
        } else if (a == 180) {
            c = -1;
            s = 0;
        } else if (a == 270) {
            c = 0;
            s = -1;
        }

        const long long lx[4] = {0, w, w, 0};
        const long long ly[4] = {0, 0, h, h};
        long long mx[4] = {0, 0, 0, 0};
        long long my[4] = {0, 0, 0, 0};

        for (int k = 0; k < 4; ++k) {
            const long long ax = (lx[k] - ox) * sx;
            const long long ay = (ly[k] - oy) * sy;
            const long long rx = ax * c - ay * s;
            const long long ry = ax * s + ay * c;
            mx[k] = px + rx;
            my[k] = py + ry;
        }

        long long minx = mx[0];
        long long maxx = mx[0];
        long long miny = my[0];
        long long maxy = my[0];
        for (int k = 1; k < 4; ++k) {
            minx = std::min(minx, mx[k]);
            maxx = std::max(maxx, mx[k]);
            miny = std::min(miny, my[k]);
            maxy = std::max(maxy, my[k]);
        }

        std::cout << nom << " COINS";
        for (int k = 0; k < 4; ++k) {
            std::cout << ' ' << mx[k] << ' ' << my[k];
        }
        std::cout << '\n';
        std::cout << nom << " BOITE " << minx << ' ' << miny << ' ' << maxx << ' ' << maxy << '\n';
    }

    std::cout << "REFUSES " << refuses << '\n';
    return 0;
}
