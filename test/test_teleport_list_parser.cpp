#include "teleport_list_parser.h"

using namespace std;

int main()
{
    auto parser = new TeleportListParser();
    parser->Init();

    unordered_map<string, string> map;
    map["a"] = "a";
    map["c"] = "c";
    map["d"] = "d";
    map["b"] = "b";
    // for (auto &[k, v] : map) {
        // cout << k << ", " << v << endl;
    // for (auto it = map.begin(); it != map.end(); it++) {
    //     cout << it->first << ", " << it->second << endl;
    // }
}