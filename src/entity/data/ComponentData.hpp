#pragma once 
#include "IData.hpp"
#include <string>
#include <utility>
#include <vector>

class ComponentData : public IData
{
public:
    std::string name;
    bool is_active = false;
    std::vector<std::pair<std::string, std::string>> variables;
};
