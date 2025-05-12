#pragma once

#include "IEditor.hpp"

#include <string>
#include <unordered_map>

enum ItemClicked{
    NONE,
    PREFAB,
    DATABASE,
    WORLD
};

class Asset;

class PrefabEditor : public IEditor {
public:
    PrefabEditor();
    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;

    //popups
    void prefab_popup();
    void database_popup();
    void world_popup();
private:
    ItemClicked m_item_clicked = NONE;
    std::unordered_map<std::string, Asset> m_worlds;
};
