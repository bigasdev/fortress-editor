#include "RendererViewer.hpp"

#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../../core/InputManager.hpp"
#include "../../tools/Logger.hpp"
#include "../../res/Res.hpp"
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
}

vec2 RendererViewer::get_coordinate() {
  vec2 coord = {0.0f, 0.0f};
  coord.x = (Mouse::get_mouse_pos().x - m_pos.x) + local_pos.x;
  coord.y = (Mouse::get_mouse_pos().y - m_pos.y) + local_pos.y;
  return coord;
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

  for(auto& module : m_modules) {
    module->update();
  }
}

void RendererViewer::draw(const vec2& size, const vec2& pos) {
  m_size = size;
  m_pos = pos;

  //bg drawing
  g_renderer->draw_rect({static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y)}, {50, 50, m_mouse_on_area ? 85 : 65, 255}, true);

  //grid drawing
  for (auto& cel : m_cels) {
    if ((cel.x - local_pos.x) > (size.x - 16) || (cel.y - local_pos.y) > (size.y - 16) || (cel.x-local_pos.x) < 0 || (cel.y-local_pos.y) < 0) {
      continue;
    }
    g_renderer->draw_rect({(cel.x-local_pos.x) + pos.x, (cel.y-local_pos.y) + pos.y, cel.w, cel.h}, cel.color, true);
  }

  for (auto& point : m_points) {
    g_renderer->draw_rect({(point.x - local_pos.x) + pos.x, (point.y - local_pos.y) + pos.y, 16, 16}, {0, 255, 0, 255}, true);
  }
  for (auto& module : m_modules) {
    module->draw({(local_pos.x) + pos.x, (local_pos.y) + pos.y});
  }


  //info drawing
}
