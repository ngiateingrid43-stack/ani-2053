#include <cmath>
#include <iostream>

int main() {
    const double pi = 3.141592653589793;

    int n = 0;
    std::cin >> n;

    int visibles = 0;
    int refuses = 0;

    for (int i = 0; i < n; ++i) {
        long long r = 0;
        long long segments = 0;
        if (!(std::cin >> r >> segments)) {
            break;
        }

        if (segments < 3) {
            std::cout << r << ' ' << segments << " REFUSE\n";
            ++refuses;
            continue;
        }

        const double g = static_cast<double>(r) * (1.0 - std::cos(pi / static_cast<double>(segments)));
        const long long ecart = static_cast<long long>(std::floor(g * 1000.0));

        if (g == 0.0) {
            std::cout << r << ' ' << segments << ' ' << ecart << " JAMAIS\n";
            continue;
        }

        const long long zoom = static_cast<long long>(std::ceil(100.0 / g));
        const bool visible = (zoom <= 100);
        if (visible) {
            ++visibles;
        }
        std::cout << r << ' ' << segments << ' ' << ecart << ' ' << zoom << ' '
                  << (visible ? "VISIBLE" : "INVISIBLE") << '\n';
    }

    std::cout << "VISIBLES " << visibles << '\n';
    std::cout << "REFUSES " << refuses << '\n';
    return 0;
}
