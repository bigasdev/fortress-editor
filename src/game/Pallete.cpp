#include "Pallete.hpp"
#include "../core/InputManager.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../renderer/Camera.hpp"
#include "../renderer/Renderer.hpp"
#include "../res/Res.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "../tools/Mouse.hpp"
#include "Fini.hpp"
#include "SDL_keycode.h"

vec2 start_pos = {350, 30};

void Pallete::init() {
  m_palettes.clear();
  if (FUtils::folder_exists(
          g_fini->get_value<std::string>("editor", "project_folder"))) {

    // getting all the files with .aseprite extension inside the res folder
    FUtils::get_all_files_in_folder(
        g_fini->get_value<std::string>("editor", "project_folder") + "\\res",
        m_palettes);

  } else {
  }

  if (m_cells.empty()) {
    for (int i = 0; i < 48; ++i) {
      for (int j = 0; j < 48; ++j) {
        GridCell cell;
        cell.grid.x =
            start_pos.x + i * base_px_w * zoom * g_camera->get_game_scale();
        cell.grid.y =
            start_pos.y + j * base_px_w * zoom * g_camera->get_game_scale();
        cell.grid.w = base_px_w * zoom * g_camera->get_game_scale();
        cell.grid.h = base_px_w * zoom * g_camera->get_game_scale();
        cell.color = Col(255, 255, 255, 25);
        cell.m_selected_color = Col(255, 255, 255, 125);
        m_cells[{i, j}] = cell;
      }
    }
  }
}

void Pallete::update() {
  Logger::log("Pallete update");
  // changing cells sizes by 8
  if (g_input_manager->get_key_press(SDLK_q)) {
    if (zoom > 1) {
      zoom--;
      for (auto &cell : m_cells) {
        cell.second.grid.x = start_pos.x + cell.first.x * base_px_w * zoom *
                                               g_camera->get_game_scale();
        cell.second.grid.y = start_pos.y + cell.first.y * base_px_w * zoom *
                                               g_camera->get_game_scale();
        cell.second.grid.w = base_px_w * zoom * g_camera->get_game_scale();
        cell.second.grid.h = base_px_w * zoom * g_camera->get_game_scale();
      }
    }
  }

  if (g_input_manager->get_key_press(SDLK_e)) {
    if (zoom < 8) {
      zoom++;
      for (auto &cell : m_cells) {
        cell.second.grid.x = start_pos.x + cell.first.x * base_px_w * zoom *
                                               g_camera->get_game_scale();
        cell.second.grid.y = start_pos.y + cell.first.y * base_px_w * zoom *
                                               g_camera->get_game_scale();
        cell.second.grid.w = base_px_w * zoom * g_camera->get_game_scale();
        cell.second.grid.h = base_px_w * zoom * g_camera->get_game_scale();
      }
    }
  }
}

void Pallete::side_draw() {
  for (const auto &pallete : m_palettes) {
    if (FUtils::get_file_extension(pallete, ".aseprite")) {
      if (ImGui::Button((FUtils::get_file_name(pallete)).c_str())) {
        m_current_palette = pallete;
        m_current_image = g_res->load_aseprite(m_current_palette);
      }
    }
  }
}

void Pallete::draw() {
  if (m_current_palette != "") {

    if (m_current_image != nullptr) {

      g_renderer->draw_raw_sheet(m_current_image, start_pos);

      for (const auto &cell : m_cells) {
        if (Mouse::is_at_area(cell.second.grid,
                              base_px_w * zoom * g_camera->get_game_scale(),
                              base_px_w * zoom * g_camera->get_game_scale())) {
          Rect cell_rect = cell.second.grid;
          g_renderer->draw_rect(cell.second.grid, cell.second.m_selected_color,
                                true);
        } else {
          g_renderer->draw_rect(cell.second.grid, cell.second.color, true);
        }
      }

    } else {
    }
  }
}

void Pallete::clean() { Logger::log("Tabs cleaned"); }
