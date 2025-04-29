#pragma once 

#include "ITab.hpp"
#include <string>

class GameProfileTab : public ITab
{
public:
    GameProfileTab() = default;
    GameProfileTab(const std::string& _name);

    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;
    void save() override;
};
