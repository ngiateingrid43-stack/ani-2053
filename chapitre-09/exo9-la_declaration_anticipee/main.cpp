#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>

using namespace std;

int main() {
    int U;
    if (!(cin >> U)) return 0;
    cin.ignore();

    vector<string> order;
    map<string, bool> needs_include;
    
    for (int i = 0; i < U; ++i) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string type, usage;
        ss >> type >> usage;
        if (needs_include.find(type) == needs_include.end()) {
            order.push_back(type);
            needs_include[type] = false;
        }
        if (usage == "VALEUR" || usage == "BASE" || usage == "CORPS" || usage == "TAILLE") {
            needs_include[type] = true;
        }
    }

    int A;
    cin >> A;
    set<string> included;
    for (int i = 0; i < A; ++i) {
        string t;
        cin >> t;
        included.insert(t);
    }

    int to_include = 0, to_anticipate = 0, removed = 0;
    for (const string& t : order) {
        if (needs_include[t]) {
            cout << t << " INCLURE\n";
            to_include++;
        } else {
            cout << t << " ANTICIPER\n";
            to_anticipate++;
            if (included.count(t)) removed++;
        }
    }

    cout << "INCLURE " << to_include << "\n";
    cout << "ANTICIPER " << to_anticipate << "\n";
    cout << "INCLUSIONS_RETIREES " << removed << "\n";

    return 0;
}
