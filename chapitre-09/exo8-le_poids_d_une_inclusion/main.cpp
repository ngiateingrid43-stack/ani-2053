#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>

using namespace std;

bool ends_with(const string& s, const string& suffix) {
    if (s.length() < suffix.length()) return false;
    return s.substr(s.length() - suffix.length()) == suffix;
}

int main() {
    int N;
    if (!(cin >> N)) return 0;
    cin.ignore();

    vector<string> files(N);
    map<string, vector<string>> deps;
    map<string, int> file_idx;
    vector<bool> is_cpp(N);

    for (int i = 0; i < N; ++i) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string name;
        int k;
        ss >> name >> k;
        files[i] = name;
        file_idx[name] = i;
        is_cpp[i] = ends_with(name, ".cpp");
        for (int j = 0; j < k; ++j) {
            string dep;
            ss >> dep;
            deps[name].push_back(dep);
        }
    }

    auto count_recompile = [&](const string& target) {
        set<int> affected;
        vector<int> stack;
        stack.push_back(file_idx[target]);
        
        while (!stack.empty()) {
            int curr = stack.back();
            stack.pop_back();
            
            if (is_cpp[curr]) affected.insert(curr);
            
            for (int i = 0; i < N; ++i) {
                if (i == curr) continue;
                for (const string& d : deps[files[i]]) {
                    if (d == files[curr]) {
                        stack.push_back(i);
                        break;
                    }
                }
            }
        }
        return (int)affected.size();
    };

    int Q;
    cin >> Q;
    cin.ignore();
    
    for (int i = 0; i < Q; ++i) {
        string q;
        getline(cin, q);
        cout << q << " " << count_recompile(q) << "\n";
    }

    string best_h = "";
    int max_val = -1;
    for (int i = 0; i < N; ++i) {
        if (ends_with(files[i], ".h")) {
            int val = count_recompile(files[i]);
            if (val > max_val) {
                max_val = val;
                best_h = files[i];
            }
        }
    }

    if (best_h.empty()) cout << "PLUS_LOURD AUCUN\n";
    else cout << "PLUS_LOURD " << best_h << " " << max_val << "\n";

    return 0;
}
