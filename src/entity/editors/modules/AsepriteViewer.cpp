#include "AsepriteViewer.hpp"

#include "../../../core/global.hpp"
#include "../../../renderer/Renderer.hpp"
#include "../../../core/InputManager.hpp"
#include "../../../tools/Logger.hpp"
#include "../../../res/Res.hpp"
#include "../../../tools/Mouse.hpp"
#include "../../../entity/assets/IAsset.hpp"
#include "../../../core/Timer.hpp"

AsepriteViewer::AsepriteViewer(GPU_Image* ase, Asset* asset) : m_ase(ase), m_asset(asset) {
}

void AsepriteViewer::update() {
  if(g_left_click && !m_is_dragging){
    if(m_last_mouse_pos != Mouse::get_mouse_pos()){
      m_last_mouse_pos = Mouse::get_mouse_pos();
      m_current_mouse_pos = m_last_mouse_pos;
      m_is_dragging = true;
    }
  }else if(g_left_click && m_is_dragging){
    m_current_mouse_pos = Mouse::get_mouse_pos();

    if(m_asset != nullptr){
      m_asset->data["x"].value = std::to_string(static_cast<int>(m_last_mouse_pos.x - m_pos.x));
      m_asset->data["y"].value = std::to_string(static_cast<int>(m_last_mouse_pos.y - m_pos.y));
  //dividing by 3 becuase its the zoom im using rn, later change it to the actual zoom
      m_asset->data["w"].value = std::to_string(static_cast<int>(m_current_mouse_pos.x - m_last_mouse_pos.x)/3);
      m_asset->data["h"].value = std::to_string(static_cast<int>(m_current_mouse_pos.y - m_last_mouse_pos.y)/3);
    }
  }
  else if(!g_left_click && m_is_dragging){
    m_is_dragging = false;
  }
}

void AsepriteViewer::draw() {
  g_renderer->draw_raw_sheet(m_ase, m_pos);

  auto rect = Rect{m_last_mouse_pos.x, m_last_mouse_pos.y, m_current_mouse_pos.x - m_last_mouse_pos.x, m_current_mouse_pos.y - m_last_mouse_pos.y};
  g_renderer->draw_rect(rect, {0, 255, 0, 255}, false);

  g_renderer->draw_rect({m_current_mouse_pos.x, m_current_mouse_pos.y, 100, 30}, {0, 0, 255, 255}, true);
  //dividing by 3 becuase its the zoom im using rn, later change it to the actual zoom
  auto text = std::to_string(static_cast<int>(rect.w/3)) + ", " + std::to_string(static_cast<int>(rect.h/3));
  g_renderer->draw_text({m_current_mouse_pos.x, m_current_mouse_pos.y + 15}, text.c_str(), g_res->get_font("arial"), {255, 255, 255, 255}, 1, 100);
}
