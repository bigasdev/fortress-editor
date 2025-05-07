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

private:
    Asset* m_asset = nullptr;

    int m_game_width = 640;
    int m_game_height = 360;
    float m_game_scale = 3.0f;
    bool m_game_fullscreen = false;
    int m_game_fps = 60;
    float m_fixed_update_factor = 0.0166f;
    bool m_game_debug = true;
};
