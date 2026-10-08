// Exercice 9 - Conteneur et codec
// Reconnait le conteneur d'un fichier media a ses premiers octets, nomme le codec
// de chaque piste d'un MP4, et dit si le lecteur video saura l'ouvrir.
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

// Vrai si les octets b[debut..] valent le motif (et qu'ils sont donnes en nombre suffisant).
static bool motifA(const vector<int> &b, size_t debut, const vector<int> &motif) {
    if (b.size() < debut + motif.size()) return false;
    for (size_t i = 0; i < motif.size(); ++i)
        if (b[debut + i] != motif[i]) return false;
    return true;
}

// Le conteneur : les regles se testent dans cet ordre, la premiere qui s'applique decide.
static string conteneur(const vector<int> &b) {
    const size_t n = b.size();
    if (n >= 12 && motifA(b, 4, {0x66, 0x74, 0x79, 0x70})) return "MP4";
    if (n >= 4 && motifA(b, 0, {0x1A, 0x45, 0xDF, 0xA3})) return "WEBM";
    if (n >= 12 && motifA(b, 0, {0x52, 0x49, 0x46, 0x46}) && motifA(b, 8, {0x57, 0x41, 0x56, 0x45}))
        return "WAV";
    if (n >= 4 && motifA(b, 0, {0x4F, 0x67, 0x67, 0x53})) return "OGG";
    if (n >= 4 && motifA(b, 0, {0x66, 0x4C, 0x61, 0x43})) return "FLAC";
    if (n >= 3 && motifA(b, 0, {0x49, 0x44, 0x33})) return "MP3";
    if (n >= 2 && b[0] == 0xFF && b[1] >= 0xE0) return "MP3";
    return "INCONNU";
}

// Un code absent de la table garde son nom de quatre caracteres, tel quel (majuscules comptent).
static string nommerCodec(const string &code) {
    if (code == "mp4a") return "aac";
    if (code == "Opus" || code == "opus") return "opus";
    if (code == "avc1" || code == "avc3") return "h264";
    if (code == "hvc1" || code == "hev1") return "h265";
    if (code == "vp08") return "vp8";
    if (code == "vp09") return "vp9";
    if (code == "mp4v") return "mpeg4";
    if (code == ".mp3") return "mp3";
    if (code == "twos" || code == "sowt" || code == "lpcm") return "pcm";
    return code;
}

static bool codecLisible(const string &code) {
    return code == "mjpa" || code == "jpeg" || code == "MJPG" || code == "avc1" || code == "avc3" ||
           code == "hvc1" || code == "hev1" || code == "av01";
}

struct Piste {
    string type;
    string code;
};

int main() {
    int n = 0;
    cin >> n;
    int nbMp4 = 0, lisibles = 0, inconnus = 0;
    for (int i = 0; i < n; ++i) {
        string nom, hex;
        int p = 0;
        cin >> nom >> hex >> p;
        // On lit toujours toutes les pistes, meme si on les ignore.
        vector<Piste> pistes(static_cast<size_t>(p));
        for (int k = 0; k < p; ++k) cin >> pistes[static_cast<size_t>(k)].type >> pistes[static_cast<size_t>(k)].code;

        const string cont = conteneur(decoderHex(hex));
        cout << nom << " " << cont << "\n";
        if (cont == "INCONNU") ++inconnus;
        if (cont != "MP4") continue; // les pistes ne se lisent que dans un MP4

        ++nbMp4;
        for (int k = 0; k < p; ++k) {
            const Piste &pi = pistes[static_cast<size_t>(k)];
            cout << nom << " PISTE " << (k + 1) << " " << (pi.type == "vide" ? "VIDEO" : "AUDIO") << " "
                 << nommerCodec(pi.code) << "\n";
        }
        // Le lecteur regarde la premiere piste video.
        string verdict = "SANS_IMAGE";
        for (int k = 0; k < p; ++k) {
            const Piste &pi = pistes[static_cast<size_t>(k)];
            if (pi.type == "vide") {
                verdict = codecLisible(pi.code) ? "LISIBLE" : "ECHEC";
                break;
            }
        }
        cout << nom << " LECTEUR " << verdict << "\n";
        if (verdict == "LISIBLE") ++lisibles;
    }
    cout << "MP4 " << nbMp4 << "\n";
    cout << "LISIBLES " << lisibles << "\n";
    cout << "INCONNUS " << inconnus << "\n";
    return 0;
}
