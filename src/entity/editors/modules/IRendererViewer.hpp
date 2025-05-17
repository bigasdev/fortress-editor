#pragma once

#include "../../../tools/Common.hpp"

class IRendererViewer {
public:
    virtual ~IRendererViewer() = default;
    virtual void update() = 0;
    virtual void draw(const vec2& pos) = 0;
};
