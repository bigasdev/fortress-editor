#pragma once 
#include "IData.hpp"
#include "../../tools/Common.hpp"
#include <string>
#include <unordered_map>
#include "ComponentData.hpp"
#include <vector>

class EntityData : public IData
{
public:
    std::string name;
    std::string pallete_name;
    std::string group;
    vec2i sprite_size;
    vec2i collision_box = {16, 16};
    vec2i collision_offset = {0, 0};
    vec2i sprite_pos;
    vec2i sprite_offset;
    vec2i atlas_pos;

    std::unordered_map<std::string, ComponentData> components;
private:
};
