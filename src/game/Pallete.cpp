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
}

void Pallete::update() {
  if (Mouse::get_mouse_pos().x > start_pos.x) {
    if (g_left_click && mouse_pos == vec2{0, 0}) {
      mouse_pos = Mouse::get_mouse_pos();
    }

    if (g_left_click) {
      mouse_end_pos = Mouse::get_mouse_pos();
    }

    if (g_right_click) {
      mouse_pos = vec2{0, 0};
      mouse_end_pos = vec2{0, 0};
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
    } else {
    }

    // drawing the rectangle for the mouse selection
    if (mouse_pos != vec2{0, 0} && mouse_end_pos != vec2{0, 0}) {
      auto rect =
          Rect{static_cast<int>(std::min(mouse_pos.x, mouse_end_pos.x)),
               static_cast<int>(std::min(mouse_pos.y, mouse_end_pos.y)),
               static_cast<int>(std::abs(mouse_end_pos.x - mouse_pos.x)),
               static_cast<int>(std::abs(mouse_end_pos.y - mouse_pos.y))};
      g_renderer->draw_rect(rect, {255, 0, 0, 128}, false);
    }
  }
}

void Pallete::clean() { Logger::log("Tabs cleaned"); }
