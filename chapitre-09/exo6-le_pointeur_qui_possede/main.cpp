#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    int N;
    if (!(cin >> N)) return 0;
    cin.ignore();

    vector<string> lines(N);
    bool has_copy = false;
    for (int i = 0; i < N; ++i) {
        getline(cin, lines[i]);
        if (lines[i].find("COPY") == 0) has_copy = true;
    }

    if (has_copy) {
        for (int i = 0; i < N; ++i) {
            if (lines[i].find("COPY") == 0) {
                cout << "REFUSE " << (i + 1) << "\n";
            }
        }
        return 0;
    }

    map<string, int> var_obj;
    vector<int> created_order;
    int next_obj_id = 1;
    int created_count = 0, destroyed_count = 0, null_count = 0;

    for (int i = 0; i < N; ++i) {
        stringstream ss(lines[i]);
        string cmd, a, b;
        ss >> cmd;
        if (cmd == "NEW") {
            ss >> a;
            int new_id = next_obj_id++;
            created_count++;
            if (var_obj.count(a) && var_obj[a] != 0) {
                cout << "DETRUIT " << var_obj[a] << "\n";
                destroyed_count++;
            }
            var_obj[a] = new_id;
        } else if (cmd == "MOVE") {
            ss >> a >> b;
            if (var_obj.count(a) && var_obj[a] != 0) {
                if (var_obj.count(b) && var_obj[b] != 0) {
                    cout << "DETRUIT " << var_obj[b] << "\n";
                    destroyed_count++;
                }
                var_obj[b] = var_obj[a];
            } else {
                if (var_obj.count(b) && var_obj[b] != 0) {
                    cout << "DETRUIT " << var_obj[b] << "\n";
                    destroyed_count++;
                }
                var_obj[b] = 0;
            }
            var_obj[a] = 0;
        } else if (cmd == "RESET") {
            ss >> a;
            if (var_obj.count(a) && var_obj[a] != 0) {
                cout << "DETRUIT " << var_obj[a] << "\n";
                destroyed_count++;
            }
            var_obj[a] = 0;
        } else if (cmd == "USE") {
            ss >> a;
            if (!var_obj.count(a) || var_obj[a] == 0) {
                cout << "NUL " << (i + 1) << " " << a << "\n";
                null_count++;
            }
        }
    }

    for (auto it = var_obj.rbegin(); it != var_obj.rend(); ++it) {
        if (it->second != 0) {
            cout << "DETRUIT " << it->second << "\n";
            destroyed_count++;
        }
    }

    cout << "CREES " << created_count << "\n";
    cout << "DETRUITS " << destroyed_count << "\n";
    cout << "NULS " << null_count << "\n";

    return 0;
}
