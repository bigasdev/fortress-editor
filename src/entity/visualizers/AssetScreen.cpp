#include "AssetScreen.hpp"
#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../editors/FloatingButtons.hpp"
#include "../../renderer/Camera.hpp"
#include "../../renderer/Renderer.hpp"
#include "../../res/Res.hpp"
#include "../../tools/Logger.hpp"
#include "../data/EntityData.hpp"

AssetScreen::AssetScreen() {}

AssetScreen::~AssetScreen() {}

void AssetScreen::update() {
  if (g_selected_entity != nullptr) {
  }
}

void AssetScreen::root() {
  if(!g_floating_buttons->is_pressed("hide_grid")) {
    int x = 0;
    for (int i = 384; i < g_engine->get_window_size()->x - 32;
        i += (m_grid_size.x * m_zoom)) {
      for (int j = 48; j < g_engine->get_window_size()->y - 48;
          j += (m_grid_size.y * m_zoom)) {
        if (x == 1) {
          g_renderer->draw_rect({i, j, static_cast<int>(m_grid_size.x * m_zoom),
              static_cast<int>(m_grid_size.y * m_zoom)},
              {82, 250, 161, 55}, true);
          x = 0;
        } else {
          g_renderer->draw_rect({i, j, static_cast<int>(m_grid_size.x * m_zoom),
              static_cast<int>(m_grid_size.y * m_zoom)},
              {35, 205, 113, 65}, true);
          x++;
        }
      }
    }
  }
  if (g_selected_entity == nullptr)
    return;

  g_renderer->draw_rect({40-g_camera->get_camera_pos().x, -60-g_camera->get_camera_pos().y, g_selected_entity->sprite_size.x*m_zoom, g_selected_entity->sprite_size.y*m_zoom}, {0, 0, 125, 55}, true);

  g_renderer->draw_from_sheet(
      *g_res->get_texture(g_selected_entity->pallete_name),
      {static_cast<float>(40) +
           (
            g_selected_entity->sprite_offset.x * m_zoom) - g_camera->get_camera_pos().x,
        static_cast<float>(-60) +
            (
             g_selected_entity->sprite_offset.y * m_zoom) - g_camera->get_camera_pos().y},
      {g_selected_entity->atlas_pos.x, g_selected_entity->atlas_pos.y,
       g_selected_entity->sprite_size.x, g_selected_entity->sprite_size.y},
      m_zoom);

  //collision box
  if(!g_floating_buttons->is_pressed("hide_cbox")){
    g_renderer->draw_rect(
        {(static_cast<float>(40)) +
        (
         (g_selected_entity->sprite_offset.x +
          g_selected_entity->collision_offset.x) * m_zoom) - g_camera->get_camera_pos().x,
        (static_cast<float>(-60)) +
        (
         (g_selected_entity->sprite_offset.y +
          g_selected_entity->collision_offset.y) * m_zoom) - g_camera->get_camera_pos().y,
        static_cast<int>(g_selected_entity->collision_box.x * m_zoom),
        static_cast<int>(g_selected_entity->collision_box.y * m_zoom)},
        {0, 255, 0, 255}, false);
  }
}

void AssetScreen::ent() {}

void AssetScreen::ui() {}
