#pragma once

#include "../../../tools/Common.hpp"

class IRendererViewer {
public:
    virtual ~IRendererViewer() = default;
    virtual void update() = 0;
    void set_pos(const vec2& pos) { m_pos = pos; }
    virtual void draw() = 0;

    vec2 m_pos = {0.0f, 0.0f};
};
