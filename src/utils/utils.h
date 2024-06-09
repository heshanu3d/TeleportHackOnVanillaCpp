#pragma once

#include <string>
#include <vector>

namespace _utils_ {

void TraverseDirectory(const std::string& directory, std::vector<std::string>& files);
std::string GetCurrentBinaryPath();
std::string GetCurrentBinaryDirectoryPath();
std::vector<std::string> split(const std::string str, const std::string regex_str);

}
