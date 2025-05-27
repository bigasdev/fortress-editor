#pragma once 

#include "ITab.hpp"
#include "IAssetTab.hpp"
#include <string>
#include <unordered_map>

class RendererViewer;

class PrefabTab : public IAssetTab
{
public:
    PrefabTab() = default;
    PrefabTab(const std::string& _name);

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

    std::unordered_map<std::string, AssetPrefabData> m_asset_data;
    //components
    std::unordered_map<std::string, std::string> m_avaliable_components;
};
