#pragma once 

#include "../../tools/Common.hpp"
#include <vector>

struct Cel{
  int x, y, w, h;
  Col color = {0, 0, 0, 255};
};

class RendererViewer
{
  public:
    RendererViewer();
    ~RendererViewer() = default;

    void draw(const vec2& size, const vec2& pos);
  private:
    float m_zoom = 1.0f;
    vec2 m_grid = {16.0f, 16.0f};
    vec2 invisible_size = {50.0f, 50.0f};
    vec2 m_size = {0.0f, 0.0f};
    vec2 m_pos = {0.0f, 0.0f};
    vec2 local_pos = {0.0f, 0.0f};

    std::vector<Cel> m_cels;
};
