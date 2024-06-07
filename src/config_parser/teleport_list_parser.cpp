#include "teleport_list_parser.h"
#include <fstream>
#include <regex>
#include <stack>
#include <windows.h>

using namespace std;

void traverseDirectory(const std::string& directory, std::vector<std::string>& files)
{
    std::string searchPath = directory + "\\*";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                    std::string subDirectory = directory + "\\" + findData.cFileName;
                    traverseDirectory(subDirectory, files);
                }
            } else {
                std::string filePath = directory + "\\" + findData.cFileName;
                files.push_back(filePath);
            }
        } while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }
}

string GetCurrentBinaryPath()
{
    char path[MAX_PATH] = {0};
    if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
        return string(path);
    } else {
        std::cerr << "get file path failed" << std::endl;
        return "";
    }
}

string GetCurrentBinaryDirectoryPath()
{
    auto path = GetCurrentBinaryPath();
    auto pos = path.find_last_of("\\");
    if (pos != std::string::npos) {
        path.erase(pos + 1);
        return path;
    }

    std::cerr << "font file path is error" << path << std::endl;
    return "";
}

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}

shared_ptr<Category> TeleportListParser::Parse(const string &configFilePath)
{
    string line;
    fstream fs(configFilePath, ios::in);
    std::regex pattern("-*\\d*\\.\\d*");
    auto rootCategory = make_shared<Category>();

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
                    root->categories[r] = make_shared<Category>();
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
    std::stack<shared_ptr<Category>> sta;

    // if (rootCategory->categories.count("MC"))
    //     root = rootCategory->categories["MC"];
    // sta.push(root);
    sta.push(rootCategory);

    while (!sta.empty()) {
        auto node = sta.top();
        sta.pop();

        if (node->point)
            node->point->Print();

        // for (auto it = node->categories.begin(); it != node->categories.end(); it++) {
        //     sta.push(it->second);
        for (auto &[k, v] : node->categories) {
            sta.push(v);
        }
    }

    return rootCategory;
}

void TeleportListParser::Init()
{
    traverseDirectory(GetCurrentBinaryDirectoryPath() + config_dir, m_configPaths);

    for (const std::string& configPath : m_configPaths) {
        // std::cout << configPath << std::endl;
        Parse(configPath);
    }
}

