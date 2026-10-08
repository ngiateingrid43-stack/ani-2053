#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Decode une chaine hexadecimale (majuscules ou minuscules) en octets 0..255.
// Un tiret signifie qu'aucun octet n'est donne.
static vector<int> decoderHex(const string &s) {
    vector<int> octets;
    if (s == "-") return octets;
    auto valeur = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    for (size_t i = 0; i + 1 < s.size(); i += 2) {
        int h = valeur(s[i]);
        int l = valeur(s[i + 1]);
        if (h < 0 || l < 0) break;
        octets.push_back(h * 16 + l);
    }
    return octets;
}

// Vrai si les octets commencent par le motif donne (et qu'ils sont assez nombreux).
static bool commencePar(const vector<int> &b, const vector<int> &motif) {
    if (b.size() < motif.size()) return false;
    for (size_t i = 0; i < motif.size(); ++i)
        if (b[i] != motif[i]) return false;
    return true;
}

// Applique les regles dans l'ordre ; la premiere qui s'applique decide.
static string reconnaitre(long long taille, const vector<int> &b) {
    const long long n = static_cast<long long>(b.size());
    if (taille < 4) return "";                                              // 1
    if (taille >= 8 && commencePar(b, {0x89, 0x50, 0x4E, 0x47})) return "PNG";   // 2
    if (commencePar(b, {0xFF, 0xD8, 0xFF})) return "JPEG";                  // 3
    if (commencePar(b, {0x42, 0x4D})) return "BMP";                         // 4
    if (commencePar(b, {0x71, 0x6F, 0x69, 0x66})) return "QOI";             // 5
    if (commencePar(b, {0x47, 0x49, 0x46, 0x38})) return "GIF";             // 6
    if (n >= 4 && b[0] == 0x00 && b[1] == 0x00 && (b[2] == 0x01 || b[2] == 0x02) && b[3] == 0x00)
        return "ICO";                                                       // 7
    if (taille >= 10 && commencePar(b, {0x23, 0x3F})) return "HDR";         // 8
    if (commencePar(b, {0x76, 0x2F, 0x31, 0x01})) return "EXR";             // 9
    if (n >= 2 && b[0] == 0x50 && b[1] >= 0x31 && b[1] <= 0x36) {           // 10
        if (b[1] == 0x31 || b[1] == 0x34) return "PBM";
        if (b[1] == 0x32 || b[1] == 0x35) return "PGM";
        return "PPM";
    }
    if (taille >= 18 && n >= 3) {                                           // 11
        int c = b[2];
        if (c == 0x00 || c == 0x01 || c == 0x02 || c == 0x03 || c == 0x09 || c == 0x0A || c == 0x0B)
            return "TGA";
    }
    // 12 : SVG
    size_t i = 0;
    if (b.size() >= 3 && b[0] == 0xEF && b[1] == 0xBB && b[2] == 0xBF) i = 3;
    while (i < b.size() && (b[i] == 0x20 || b[i] == 0x09 || b[i] == 0x0A || b[i] == 0x0D)) ++i;
    vector<int> reste(b.begin() + static_cast<long>(i), b.end());
    if (commencePar(reste, {0x3C, 0x3F, 0x78, 0x6D, 0x6C}) || commencePar(reste, {0x3C, 0x73, 0x76, 0x67}))
        return "SVG";
    return "";                                                              // 13
}

// L'extension : texte apres le DERNIER point, en minuscules. Sans point : vide.
static string extension(const string &nom) {
    size_t p = nom.rfind('.');
    if (p == string::npos) return "";
    string e = nom.substr(p + 1);
    for (char &c : e)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return e;
}

static bool extensionJuste(const string &format, const string &ext) {
    if (ext.empty()) return false;
    if (format == "PNG") return ext == "png";
    if (format == "JPEG") return ext == "jpg" || ext == "jpeg";
    if (format == "BMP") return ext == "bmp";
    if (format == "QOI") return ext == "qoi";
    if (format == "GIF") return ext == "gif";
    if (format == "ICO") return ext == "ico" || ext == "cur";
    if (format == "HDR") return ext == "hdr";
    if (format == "EXR") return ext == "exr";
    if (format == "PBM") return ext == "pbm";
    if (format == "PGM") return ext == "pgm";
    if (format == "PPM") return ext == "ppm";
    if (format == "TGA") return ext == "tga";
    if (format == "SVG") return ext == "svg";
    return false;
}

int main() {
    int n = 0;
    if (!(cin >> n)) n = 0;
    int lus = 0, mensonges = 0, refuses = 0;
    for (int k = 0; k < n; ++k) {
        string nom, hex;
        long long taille = 0;
        cin >> nom >> taille >> hex;
        vector<int> octets = decoderHex(hex);
        string format = reconnaitre(taille, octets);
        if (format.empty()) {
            cout << nom << " REFUSE\n";
            ++refuses;
        } else if (extensionJuste(format, extension(nom))) {
            cout << nom << " " << format << " OK\n";
            ++lus;
        } else {
            cout << nom << " " << format << " MENT\n";
            ++lus;
            ++mensonges;
        }
    }
    cout << "LUS " << lus << "\n";
    cout << "MENSONGES " << mensonges << "\n";
    cout << "REFUSES " << refuses << "\n";
    return 0;
}
