#pragma once 

#include "../../tools/Common.hpp"
#include "modules/IRendererViewer.hpp"
#include <memory>
#include <vector>

struct Point{
  int x, y, w, h;
};

struct Cel : Point{
  Col color = {0, 0, 0, 255};
};

class RendererViewer
{
  public:
    RendererViewer();
    ~RendererViewer() = default;

    void update();
    void draw(const vec2& size, const vec2& pos);
    vec2 get_coordinate();

    bool is_mouse_on_area() const { return m_mouse_on_area; }

    //modules
    void add_module(std::shared_ptr<IRendererViewer> module) {
        m_modules.push_back(module);
    }
  private:

    float m_zoom = 1.0f;
    vec2 m_grid = {16.0f, 16.0f};
    vec2 invisible_size = {50.0f, 50.0f};
    vec2 m_size = {0.0f, 0.0f};
    vec2 m_pos = {0.0f, 0.0f};
    vec2 local_pos = {0.0f, 0.0f};

    std::vector<Cel> m_cels;
    std::vector<Point> m_points;

    //interactions 
    Point* m_grabbed_point = nullptr;

    bool m_mouse_on_area = false;
    bool m_is_dragging = false;

    std::vector<std::shared_ptr<IRendererViewer>> m_modules;
};
