#include "teleport_list_parser.h"
#include <locale>
#include <fstream>
#include <regex>

#include "utils.h"


using namespace std;

int main()
{
    auto parser = new TeleportListParser();
    parser->Init();

    return 0;

    vector<string> configPaths;
    string configPath;
    _utils_::TraverseDirectory(_utils_::GetCurrentBinaryDirectoryPath() + "configs", configPaths);

    for (auto & path : configPaths) {
        if (path.find("abc") != string::npos) 
            configPath = path;
    }

    fstream fs(configPath, ios::in);
    string line;
    std::regex pattern("-*\\d*\\.\\d*");
    vector<string> vec;
    while (getline(fs, line)) {
        if (!std::regex_match(line, pattern)) {
            vec.push_back(line);
        }
    }

    // unordered_map<string, string> map;
    map<string, string> map;
    cout << "---------start push into unordered_map---------" << endl;
    for (auto v: vec) {
        map[v] = v;
        cout << v << endl;
    }
    cout << "---------start push travers unordered_nap---------" << endl;
    for (auto &[k, v] : map) {
        cout << k << ", " << v << endl;
    // for (auto it = map.begin(); it != map.end(); it++) {
    //     cout << it->first << ", " << it->second << endl;
    }
    return 0;
}