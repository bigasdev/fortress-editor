#pragma once

#include "IRendererViewer.hpp"

class GPU_Image;

class AsepriteViewer : public IRendererViewer {
public:
    AsepriteViewer(GPU_Image* ase);
    ~AsepriteViewer() override = default;

    void update() override;
    void draw(const vec2& pos) override;

private:
    GPU_Image* m_ase = nullptr;
};
