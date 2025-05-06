#pragma once 

#include <string>
#include <unordered_map>

struct IData{
    std::string name;
    std::string value;
};

struct Asset{
    std::string file_name;
    std::string file_path;
    bool is_static = false;
    bool is_favorite = false;
    bool is_dirty = false;

    std::unordered_map<std::string, IData> data;
};
