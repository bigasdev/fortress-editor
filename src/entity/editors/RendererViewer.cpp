#include "RendererViewer.hpp"

#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../../core/InputManager.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/Mouse.hpp"
#include "../../core/Timer.hpp"

RendererViewer::RendererViewer() {
  m_grid = {16.0f, 16.0f};
  invisible_size = {1000.0f, 1000.0f};
  m_size = {0.0f, 0.0f};
  m_pos = {0.0f, 0.0f};
  local_pos = {-178.0f, -178.0f};

  int col = 0;
  for(int i = -invisible_size.x; i < invisible_size.x; i += m_grid.x) {
    for (int j = -invisible_size.y; j < invisible_size.y; j += m_grid.y) {
      if(col % 2 == 0) {
        m_cels.push_back({i, j, m_grid.x, m_grid.y, {50, 50, 65, 255}});
      } else {
        m_cels.push_back({i, j, m_grid.x, m_grid.y, {100, 100, 125, 255}});
      }
      col++;
    }
  }

  Point test_point = {0, 0, 16, 16};
  m_points.push_back(test_point);
}

void RendererViewer::update() {
  m_mouse_on_area = Mouse::is_at_area({m_pos.x, m_pos.y, m_size.x, m_size.y}, 16, 16);
  if(!m_mouse_on_area)return;
  auto dt = Timer::get_dt();

  local_pos.x -= g_input_manager->get_raw_axis().x * dt * 100.0f;
  local_pos.y -= g_input_manager->get_raw_axis().y * dt * 100.0f;

  if(m_grabbed_point){
    m_grabbed_point->x = Mouse::get_mouse_pos().x - (m_pos.x + local_pos.x);
    m_grabbed_point->y = Mouse::get_mouse_pos().y - (m_pos.y + local_pos.y);
  }

  if(g_left_click){
    for(auto& point : m_points) {
      Logger::log("Point: " + std::to_string(point.x) + " " + std::to_string(point.y) + " " + std::to_string(point.w) + " " + std::to_string(point.h));
      if(Mouse::is_at_area({point.x, point.y, point.w, point.h}, 16, 16)) {
        Logger::log("Point grabbed: " + std::to_string(point.x) + " " + std::to_string(point.y) + " " + std::to_string(point.w) + " " + std::to_string(point.h));
        m_grabbed_point = &point;
        break;
      } else {
        m_grabbed_point = nullptr;
      }
    }
  }
}

void RendererViewer::draw(const vec2& size, const vec2& pos) {
  m_size = size;
  m_pos = pos;

  //bg drawing
  g_renderer->draw_rect({pos.x, pos.y, size.x, size.y}, {50, 50, m_mouse_on_area ? 85 : 65, 255}, true);

  //grid drawing
  for (auto& cel : m_cels) {
    if ((cel.x - local_pos.x) > (size.x - 16) || (cel.y - local_pos.y) > (size.y - 16) || (cel.x-local_pos.x) < 0 || (cel.y-local_pos.y) < 0) {
      continue;
    }
    g_renderer->draw_rect({(cel.x-local_pos.x) + pos.x, (cel.y-local_pos.y) + pos.y, cel.w, cel.h}, cel.color, true);
  }

  //entity drawing
  g_renderer->draw_rect({(m_pos.x - local_pos.x)+m_points[0].x, (m_pos.y - local_pos.y)+m_points[0].y, 16, 16}, {255, 0, 0, 255}, true);

  //info drawing
}
