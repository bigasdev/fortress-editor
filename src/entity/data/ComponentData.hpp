#pragma once 
#include "IData.hpp"
#include <string>
#include <utility>
#include <vector>

struct Component{
  std::string name;
  char val[128] = "";
};

class ComponentData : public IData
{
public:
    std::string name;
    bool is_active = false;
    std::vector<std::pair<std::string, Component>> variables;
};
