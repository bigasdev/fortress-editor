#include "RendererViewer.hpp"

#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../../tools/Logger.hpp"

RendererViewer::RendererViewer() {
  m_grid = {16.0f, 16.0f};
  invisible_size = {200.0f, 200.0f};
  m_size = {0.0f, 0.0f};
  m_pos = {0.0f, 0.0f};
  local_pos = {0.0f, 0.0f};

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

void RendererViewer::draw(const vec2& size, const vec2& pos) {
  m_size = size;
  m_pos = pos;

  //bg drawing
  g_renderer->draw_rect({pos.x, pos.y, size.x, size.y}, {50, 50, 65, 255}, true);

  //grid drawing
  for (auto& cel : m_cels) {
    if (cel.x > size.x || cel.y > size.y || cel.x < -16 || cel.y < -16) {
      continue;
    }
    g_renderer->draw_rect({cel.x + pos.x, cel.y + pos.y, cel.w, cel.h}, cel.color, true);
  }
}
