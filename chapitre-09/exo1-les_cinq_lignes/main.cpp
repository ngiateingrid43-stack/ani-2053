#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string remove_spaces(const string& s) {
    string res = "";
    for (char c : s) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            res += c;
        }
    }
    return res;
}

string remove_comments(const string& s) {
    size_t pos = s.find("//");
    if (pos != string::npos) {
        return s.substr(0, pos);
    }
    return s;
}

int main() {
    string className;
    if (!getline(cin, className)) return 0;

    vector<string> members(5, "ABSENT");
    string line;
    
    while (getline(cin, line)) {
        string cleanLine = remove_spaces(remove_comments(line));
        if (cleanLine.empty()) continue;

        string state = "ECRIT";
        if (cleanLine.length() >= 8 && cleanLine.substr(cleanLine.length() - 8) == "=delete;") state = "SUPPRIME";
        else if (cleanLine.length() >= 9 && cleanLine.substr(cleanLine.length() - 9) == "=default;") state = "DEFAUT";

        string prefixDtor = "~" + className + "(";
        string prefixCopy = className + "(const" + className + "&";
        string prefixCopyAssign = className + "&operator=(const" + className + "&";
        string prefixMove = className + "(" + className + "&&";
        string prefixMoveAssign = className + "&operator=(" + className + "&&";

        bool isDtor = cleanLine.find(prefixDtor) != string::npos;
        bool isCopy = cleanLine.find(prefixCopy) == 0 && (cleanLine.length() <= prefixCopy.length() || cleanLine[prefixCopy.length()] != '&');
        bool isCopyAssign = cleanLine.find(prefixCopyAssign) == 0 && (cleanLine.length() <= prefixCopyAssign.length() || cleanLine[prefixCopyAssign.length()] != '&');
        bool isMove = cleanLine.find(prefixMove) == 0;
        bool isMoveAssign = cleanLine.find(prefixMoveAssign) == 0;

        if (isDtor) members[0] = state;
        else if (isCopy) members[1] = state;
        else if (isCopyAssign) members[2] = state;
        else if (isMove) members[3] = state;
        else if (isMoveAssign) members[4] = state;
    }

    cout << "DESTRUCTEUR " << members[0] << "\n";
    cout << "COPIE " << members[1] << "\n";
    cout << "AFFECTATION_COPIE " << members[2] << "\n";
    cout << "DEPLACEMENT " << members[3] << "\n";
    cout << "AFFECTATION_DEPLACEMENT " << members[4] << "\n";

    bool hasDtor = members[0] != "ABSENT";
    bool hasCopy = members[1] != "ABSENT";
    bool hasCopyAssign = members[2] != "ABSENT";
    bool hasMove = members[3] != "ABSENT";
    bool hasMoveAssign = members[4] != "ABSENT";

    string regle = "INCOMPLETE";
    if (!hasDtor && !hasCopy && !hasCopyAssign && !hasMove && !hasMoveAssign) {
        regle = "ZERO";
    } else if (hasDtor && hasCopy && hasCopyAssign && hasMove && hasMoveAssign) {
        regle = "CINQ";
    } else if (hasDtor && hasCopy && hasCopyAssign && !hasMove && !hasMoveAssign) {
        regle = "TROIS";
    }
    cout << "REGLE " << regle << "\n";

    if (regle == "INCOMPLETE") {
        bool hasAnyMove = hasMove || hasMoveAssign;
        vector<string> order = {"DESTRUCTEUR", "COPIE", "AFFECTATION_COPIE", "DEPLACEMENT", "AFFECTATION_DEPLACEMENT"};
        for (int i = 0; i < 5; ++i) {
            if (members[i] == "ABSENT") {
                if (hasAnyMove || i < 3) {
                    cout << "MANQUE " << order[i] << "\n";
                }
            }
        }
    }

    cout << "POSSEDE " << (members[0] == "ECRIT" ? "OUI" : "NON") << "\n";
    cout << "UNIQUE " << (members[1] == "SUPPRIME" && (members[3] == "ECRIT" || members[3] == "DEFAUT") ? "OUI" : "NON") << "\n";

    return 0;
}
