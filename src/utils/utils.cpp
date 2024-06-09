#include "utils.h"
#include <windows.h>
#include <regex>
#include <iostream>

// using namespace std;

namespace _utils_ {

void TraverseDirectory(const std::string& directory, std::vector<std::string>& files)
{
    std::string searchPath = directory + "\\*";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                    std::string subDirectory = directory + "\\" + findData.cFileName;
                    TraverseDirectory(subDirectory, files);
                }
            } else {
                std::string filePath = directory + "\\" + findData.cFileName;
                files.push_back(filePath);
            }
        } while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }
}

std::string GetCurrentBinaryPath()
{
    char path[MAX_PATH] = {0};
    if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
        return std::string(path);
    } else {
        std::cerr << "get file path failed" << std::endl;
        return "";
    }
}

std::string GetCurrentBinaryDirectoryPath()
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

}