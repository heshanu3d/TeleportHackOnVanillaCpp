#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

struct Category;

struct Point {
    Point(std::string n, float fx, float fy, float fz) : name(n), x(fx), y(fy), z(fz) {
        std::cout << "created " << name << std::endl;
    }
    float x;
    float y;
    float z;
    std::string name;
    void Print() {
        std::cout << name << ":"
                  << x    << ","
                  << y    << ","
                  << z    << std::endl;
    }
};

struct Category {
    std::unordered_map<std::string, std::shared_ptr<Category>> categories;
    std::shared_ptr<Point> point = nullptr;
};

class TeleportListParser {
public:
    TeleportListParser() {};
    void Init();
private:
    std::shared_ptr<Category> Parse(const std::string &configFilePath);
    std::vector<std::shared_ptr<Category>> m_root;
    std::vector<std::string> m_configPaths;
    std::string config_dir = "configs\\";
};

// Category root;
