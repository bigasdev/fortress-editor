#include "Pallete.hpp"
#include "../core/InputManager.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
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
    for (int i = 0; i < 16; ++i) {
      GridCell cell;
      cell.grid = {start_pos.x + (i * 32), start_pos.y, 32, 32};
      cell.color = {255, 255, 255, 15};             // default color
      cell.m_selected_color = {255, 255, 255, 125}; // selected color
      m_cells[i] = cell;
    }
  }
}

void Pallete::update() {
  Logger::log("Pallete update");
  // changing cells sizes by 8
  if (g_input_manager->get_key_press(SDLK_q)) {
    for (auto &cell : m_cells) {
      cell.second.grid.w -= 16;
      cell.second.grid.h -= 16;
    }
  }

  if (g_input_manager->get_key_press(SDLK_e)) {
    for (auto &cell : m_cells) {
      cell.second.grid.x += 16;
      cell.second.grid.y += 16;
      cell.second.grid.w += 16;
      cell.second.grid.h += 16;
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
        if (Mouse::is_at_area(cell.second.grid, 16, 16)) {
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
