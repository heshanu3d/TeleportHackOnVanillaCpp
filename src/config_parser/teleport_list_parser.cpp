#include "teleport_list_parser.h"
#include <fstream>
#include <stack>
#include <regex>

#include "utils.h"

using namespace std;
using namespace _utils_;

void Category::Traverse(std::function<void(shared_ptr<Category>)> action)
{
    stack<shared_ptr<Category>> sta;
    sta.push(shared_from_this());

    while (!sta.empty()) {
        auto node = sta.top();
        sta.pop();

        action(node);

        for (auto &[k, v] : node->categories) {
            sta.push(v);
        }
    }
}



// void Category::Traverse(std::function<void(shared_ptr<Category>)> action = [](shared_ptr<Category> node){ node->Print(); })
// {
//     stack<shared_ptr<Category>> sta;
//     sta.push(shared_from_this());

//     while (!sta.empty()) {
//         auto node = sta.top();
//         sta.pop();

//         action(node);

//         for (auto &[k, v] : node->categories) {
//             sta.push(v);
//         }
//     }
// }

shared_ptr<Category> TeleportListParser::Parse(const std::string &configFilePath)
{
    string line;
    fstream fs(configFilePath, ios::in);
    std::regex pattern("-*\\d*\\.\\d*");
    auto fileName = configFilePath;
    auto pos = fileName.find_last_of("\\");
    if (pos != string::npos)
        fileName.erase(0, pos + 1);
    auto rootCategory = make_shared<Category>(fileName == configFilePath ? "" : fileName);

    shared_ptr<Category> root;
    string name;
    float f[3];
    int idx = 0;
    while (getline(fs, line)) {
        if (std::regex_match(line, pattern)) {
            f[idx++] = stof(line);
        } else {
            if (idx == 3) {
                root->point = make_shared<Point>(name, f[0], f[1], f[2]);
                idx = 0;
            }
            name = line;
            auto res = split(name, "\\s*\\-\\s*");

            root = rootCategory;
            
            for (auto r : res) {
                if (!root->categories.count(r)) {
                    root->categories[r] = make_shared<Category>(r, root->depth);
                }
                root = root->categories[r];
            }
        }
    }
    if (idx == 3) {
        root->point = make_shared<Point>(name, f[0], f[1], f[2]);
        idx = 0;
    }

    cout << "[" << configFilePath << "] start traverse root node" << endl;

    // if (rootCategory->categories.count("MC")) {
    //     root = rootCategory->categories["MC"];
    //     root->Traverse();
    // }
#ifdef DEBUG
    rootCategory->Traverse();
#endif

    return rootCategory;
}

void TeleportListParser::Init()
{
    _utils_::TraverseDirectory(GetCurrentBinaryDirectoryPath() + config_dir, m_configPaths);

    for (const std::string& configPath : m_configPaths) {
        // std::cout << configPath << std::endl;
        m_root.push_back(Parse(configPath));
    }
}

