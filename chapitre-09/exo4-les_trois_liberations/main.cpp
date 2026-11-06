#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    int S, V, F;
    if (!(cin >> S >> V >> F)) return 0;

    int N;
    cin >> N;

    vector<int> tableau;
    int next_texture_id = 1;
    int total_created = 0;
    vector<bool> is_freed(1, false);

    for (int op = 1; op <= N; ++op) {
        string cmd;
        cin >> cmd;
        if (cmd == "AJOUTE") {
            tableau.push_back(next_texture_id);
            is_freed.push_back(false);
            next_texture_id++;
            total_created++;
        } else if (cmd == "SUPPRIME") {
            int i;
            cin >> i;
            int tex_id = tableau[i];
            for (int r = 0; r < S; ++r) {
                if (is_freed[tex_id]) {
                    cout << "PLANTAGE " << op << " TEXTURE " << tex_id << "\n";
                    return 0;
                }
                is_freed[tex_id] = true;
            }
            tableau.erase(tableau.begin() + i);
        } else if (cmd == "VIDE") {
            for (int tex_id : tableau) {
                for (int r = 0; r < V; ++r) {
                    if (is_freed[tex_id]) {
                        cout << "PLANTAGE " << op << " TEXTURE " << tex_id << "\n";
                        return 0;
                    }
                    is_freed[tex_id] = true;
                }
            }
            tableau.clear();
        }
    }

    for (int tex_id : tableau) {
        for (int r = 0; r < F; ++r) {
            if (is_freed[tex_id]) {
                cout << "PLANTAGE FIN TEXTURE " << tex_id << "\n";
                return 0;
            }
            is_freed[tex_id] = true;
        }
    }

    int fuites = 0;
    vector<int> perdues;
    for (int i = 1; i <= total_created; ++i) {
        if (!is_freed[i]) {
            fuites++;
            perdues.push_back(i);
        }
    }

    cout << "CREEES " << total_created << "\n";
    cout << "FUITES " << fuites << "\n";
    cout << "PERDUES";
    if (perdues.empty()) cout << " AUCUN";
    else for (int p : perdues) cout << " " << p;
    cout << "\n";

    return 0;
}
