#pragma once 

#include <string>
#include "TabsCommon.hpp"

struct AssetPrefabData {
    std::string name;
    int x;
    int y;
    int w;
    int h;
};

class ITab{
public:
    virtual ~ITab(){}
    virtual void open() = 0;
    virtual void show() = 0;
    virtual void update() = 0;
    virtual void dispose() = 0;
    virtual void draw() = 0;
    virtual void reload() = 0;
    virtual void save() = 0;

    bool is_open = false;
    bool is_visible = false;
    bool is_dirty = false;
    std::string name = "Tab";
};
