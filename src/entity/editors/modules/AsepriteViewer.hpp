#pragma once

#include "IRendererViewer.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class GPU_Image;
class Asset;
class AssetPrefabData;


class AsepriteViewer : public IRendererViewer {
public:
    AsepriteViewer(GPU_Image* ase, Asset* asset);
    ~AsepriteViewer() override = default;

    void update() override;
    void draw() override;

    //data 
    void add_data(const std::string& name, AssetPrefabData* data);

private:
    GPU_Image* m_ase = nullptr;
    vec2 m_last_mouse_pos;
    vec2 m_current_mouse_pos;

    bool m_is_dragging = false;

    //data 
    Asset* m_asset = nullptr;
    std::vector<AssetPrefabData*> m_asset_data;
};
