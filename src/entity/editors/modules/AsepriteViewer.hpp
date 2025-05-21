#pragma once

#include "IRendererViewer.hpp"

class GPU_Image;
class Asset;

class AsepriteViewer : public IRendererViewer {
public:
    AsepriteViewer(GPU_Image* ase, Asset* asset);
    ~AsepriteViewer() override = default;

    void update() override;
    void draw() override;

private:
    GPU_Image* m_ase = nullptr;
    vec2 m_last_mouse_pos;
    vec2 m_current_mouse_pos;

    bool m_is_dragging = false;

    //data 
    Asset* m_asset = nullptr;
};
