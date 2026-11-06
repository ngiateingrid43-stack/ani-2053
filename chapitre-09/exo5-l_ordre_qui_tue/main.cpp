#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

struct Objet {
    string nom;
    vector<string> dependances;
    bool libere = false;
};

int main() {
    vector<Objet> objets;
    map<string, int> obj_index;
    vector<int> bloc_courant;

    string ligne;
    while (getline(cin, ligne)) {
        if (ligne.empty()) continue;
        if (ligne[0] == '{') {
            bloc_courant.push_back(-1);
        } else if (ligne[0] == '}') {
            while (!bloc_courant.empty() && bloc_courant.back() != -1) {
                int idx = bloc_courant.back();
                bloc_courant.pop_back();
                if (!objets[idx].libere) {
                    for (const string& dep : objets[idx].dependances) {
                        int dep_idx = obj_index[dep];
                        if (objets[dep_idx].libere) {
                            cout << "PLANTAGE " << objets[idx].nom << " " << dep << "\n";
                            return 0;
                        }
                    }
                    objets[idx].libere = true;
                    cout << "DETRUIT " << objets[idx].nom << "\n";
                }
            }
            if (!bloc_courant.empty()) bloc_courant.pop_back();
        } else {
            size_t espace1 = ligne.find(' ');
            string cmd = ligne.substr(0, espace1);
            string args = (espace1 != string::npos) ? ligne.substr(espace1 + 1) : "";

            if (cmd == "DECL") {
                Objet o;
                o.nom = args;
                objets.push_back(o);
                obj_index[args] = objets.size() - 1;
                if (!bloc_courant.empty() && bloc_courant.back() == -1) {
                    bloc_courant.back() = objets.size() - 1;
                } else {
                    bloc_courant.push_back(objets.size() - 1);
                }
            } else if (cmd == "DEPEND") {
                size_t espace2 = args.find(' ');
                string a = args.substr(0, espace2);
                string b = args.substr(espace2 + 1);
                objets[obj_index[a]].dependances.push_back(b);
            } else if (cmd == "FERME") {
                int idx = obj_index[args];
                if (!objets[idx].libere) {
                    for (const string& dep : objets[idx].dependances) {
                        int dep_idx = obj_index[dep];
                        if (objets[dep_idx].libere) {
                            cout << "PLANTAGE " << objets[idx].nom << " " << dep << "\n";
                            return 0;
                        }
                    }
                    objets[idx].libere = true;
                    cout << "FERME " << objets[idx].nom << "\n";
                }
            }
        }
    }

    while (!bloc_courant.empty()) {
        int idx = bloc_courant.back();
        bloc_courant.pop_back();
        if (idx >= 0 && !objets[idx].libere) {
            for (const string& dep : objets[idx].dependances) {
                int dep_idx = obj_index[dep];
                if (objets[dep_idx].libere) {
                    cout << "PLANTAGE " << objets[idx].nom << " " << dep << "\n";
                    return 0;
                }
            }
            objets[idx].libere = true;
            cout << "DETRUIT " << objets[idx].nom << "\n";
        }
    }

    cout << "PROPRE\n";
    return 0;
}
