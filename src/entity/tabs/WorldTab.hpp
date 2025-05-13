#pragma once 

#include "ITab.hpp"
#include "IAssetTab.hpp"
#include <string>

class RendererViewer;

class WorldTab : public IAssetTab
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
    void add_asset(Asset* asset) override;

private:
    Asset* m_asset = nullptr;
    RendererViewer* m_viewer = nullptr;
};
