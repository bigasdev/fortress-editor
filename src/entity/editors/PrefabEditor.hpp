#pragma once

#include "IEditor.hpp"

#include <string>

enum ItemClicked{
    NONE,
    PREFAB,
    DATABASE,
    WORLD
};

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
    void create_world_tab(const std::string& name);
    void world_popup();
private:
    ItemClicked m_item_clicked = NONE;
};
