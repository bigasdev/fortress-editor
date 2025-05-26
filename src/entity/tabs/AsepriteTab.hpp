#pragma once 

#include "IAssetTab.hpp"
#include "ITab.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class RendererViewer;
class AsepriteViewer;
class GPU_Image;

class AsepriteTab : public IAssetTab{
public:
    AsepriteTab() = default;
    AsepriteTab(const std::string& _name);
    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;
    void save() override;

    void add_asset(Asset* asset) override;
private:
    std::string m_file_path;
    std::string m_file_name;
    GPU_Image* m_ase = nullptr;
    Asset* m_asset = nullptr;
    RendererViewer* m_viewer = nullptr;
    
    std::shared_ptr<AsepriteViewer> m_aseprite_viewer = nullptr;
    std::unordered_map<std::string, AssetPrefabData> m_asset_data;
};
