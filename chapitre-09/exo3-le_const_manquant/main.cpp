#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

using namespace std;

struct Method {
    string name;
    bool is_const;
    bool modifies;
    vector<string> calls;
    bool could_be_const = false;
    bool is_root = false;
};

int main() {
    int n;
    if (!(cin >> n)) return 0;
    cin.ignore();

    vector<Method> methods(n);
    map<string, int> name_to_idx;

    for (int i = 0; i < n; ++i) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string name, declaree, modifie;
        int k;
        ss >> name >> declaree >> modifie >> k;
        methods[i].name = name;
        methods[i].is_const = (declaree == "const");
        methods[i].modifies = (modifie == "1");
        for (int j = 0; j < k; ++j) {
            string called;
            ss >> called;
            methods[i].calls.push_back(called);
        }
        name_to_idx[name] = i;
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < n; ++i) {
            if (methods[i].is_const || methods[i].modifies) continue;
            
            bool all_ok = true;
            for (const string& c : methods[i].calls) {
                int idx = name_to_idx[c];
                if (!methods[idx].is_const) {
                    all_ok = false;
                    break;
                }
            }
            if (all_ok) {
                methods[i].could_be_const = true;
            }
        }
    }

    vector<string> pourrait;
    for (int i = 0; i < n; ++i) {
        if (methods[i].could_be_const) pourrait.push_back(methods[i].name);
    }

    cout << "POURRAIT";
    if (pourrait.empty()) cout << " AUCUN";
    else for (const string& s : pourrait) cout << " " << s;
    cout << "\n";

    vector<string> racines;
    for (int i = 0; i < n; ++i) {
        if (!methods[i].could_be_const) continue;
        bool is_root = true;
        for (const string& c : methods[i].calls) {
            int idx = name_to_idx[c];
            if (!methods[idx].is_const) {
                is_root = false;
                break;
            }
        }
        if (is_root) racines.push_back(methods[i].name);
    }

    cout << "RACINES";
    if (racines.empty()) cout << " AUCUN";
    else for (const string& s : racines) cout << " " << s;
    cout << "\n";

    int passes = 0;
    vector<bool> declared(n, false);
    for (int i = 0; i < n; ++i) if (methods[i].is_const) declared[i] = true;

    int remaining = 0;
    for (int i = 0; i < n; ++i) if (methods[i].could_be_const && !declared[i]) remaining++;

    while (remaining > 0) {
        passes++;
        vector<int> to_declare;
        for (int i = 0; i < n; ++i) {
            if (methods[i].could_be_const && !declared[i]) {
                bool ready = true;
                for (const string& c : methods[i].calls) {
                    int idx = name_to_idx[c];
                    if (!declared[idx] && methods[idx].could_be_const) {
                        ready = false;
                        break;
                    }
                }
                if (ready) to_declare.push_back(i);
            }
        }
        for (int idx : to_declare) {
            declared[idx] = true;
            remaining--;
        }
        if (to_declare.empty()) break;
    }

    cout << "PASSES " << passes << "\n";
    return 0;
}
