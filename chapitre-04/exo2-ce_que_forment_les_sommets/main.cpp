#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    long long points = 0;
    long long segments = 0;
    long long triangles = 0;
    long long refuses = 0;

    for (int i = 0; i < n; ++i) {
        std::string type;
        long long s = 0;
        if (!(std::cin >> type >> s)) {
            break;
        }

        if (type == "POINTS") {
            std::cout << type << ' ' << s << ' ' << s << " POINTS 0\n";
            points += s;
        } else if (type == "LINES") {
            const long long nb = s / 2;
            std::cout << type << ' ' << s << ' ' << nb << " SEGMENTS " << s % 2 << '\n';
            segments += nb;
        } else if (type == "LINE_STRIP") {
            const long long nb = (s >= 2) ? s - 1 : 0;
            const long long reste = (s >= 2) ? 0 : s;
            std::cout << type << ' ' << s << ' ' << nb << " SEGMENTS " << reste << '\n';
            segments += nb;
        } else if (type == "TRIANGLES") {
            const long long nb = s / 3;
            std::cout << type << ' ' << s << ' ' << nb << " TRIANGLES " << s % 3 << '\n';
            triangles += nb;
        } else if (type == "TRIANGLE_STRIP" || type == "TRIANGLE_FAN") {
            const long long nb = (s >= 3) ? s - 2 : 0;
            const long long reste = (s >= 3) ? 0 : s;
            std::cout << type << ' ' << s << ' ' << nb << " TRIANGLES " << reste << '\n';
            triangles += nb;
        } else {
            std::cout << type << ' ' << s << " REFUSES\n";
            ++refuses;
        }
    }

    std::cout << "POINTS " << points << '\n';
    std::cout << "SEGMENTS " << segments << '\n';
    std::cout << "TRIANGLES " << triangles << '\n';
    std::cout << "REFUSES " << refuses << '\n';
    return 0;
}