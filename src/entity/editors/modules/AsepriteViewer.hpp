#pragma once

#include "IRendererViewer.hpp"
#include <string>
#include <unordered_map>

class GPU_Image;
class Asset;

struct AssetViewerData {
    std::string name;
    int x;
    int y;
    int w;
    int h;
};

class AsepriteViewer : public IRendererViewer {
public:
    AsepriteViewer(GPU_Image* ase, Asset* asset);
    ~AsepriteViewer() override = default;

    void update() override;
    void draw() override;

    //data 
    void add_data(const std::string& name, const AssetViewerData& data);

private:
    GPU_Image* m_ase = nullptr;
    vec2 m_last_mouse_pos;
    vec2 m_current_mouse_pos;

    bool m_is_dragging = false;

    //data 
    Asset* m_asset = nullptr;
    std::unordered_map<std::string, AssetViewerData> m_asset_data;
};
