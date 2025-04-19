#pragma once 
#include "IData.hpp"
#include <string>

class ComponentData : public IData
{
public:
    std::string name;
    bool is_active = false;
};
