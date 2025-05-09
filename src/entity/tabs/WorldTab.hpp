#pragma once 

#include "ITab.hpp"
#include <string>

class WorldTab : public ITab
{
public:
    WorldTab() = default;
    WorldTab(const std::string& _name);

    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;
    void save() override;

    void set_asset(Asset* asset);

private:
    Asset* m_asset = nullptr;
};
