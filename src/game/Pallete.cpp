#include "Pallete.hpp"
#include "../core/InputManager.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
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
#include "json.hpp"

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
        cell.name = "";
        cell.pallete = "";
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
  // loading all the data for the start + after each asset creation
  if (m_is_dirty) {
    std::string json_file_path =
        g_fini->get_value<std::string>("editor", "project_folder") +
        "\\res\\sprites\\sprites.json";
    if (FUtils::file_exists(json_file_path)) {
      std::ifstream file(json_file_path);
      nlohmann::json existing_json;
      file >> existing_json;
      file.close();

      m_grid_data.clear();
      for (const auto &item : existing_json) {
        GridData data;
        data.name = item["name"];
        data.pallete = item["palette"];
        data.x = item["x"];
        data.y = item["y"];
        data.w = item["width"];
        data.h = item["height"];
        m_grid_data.push_back(data);
      }
    }

    convert_to_grid();

    m_is_dirty = false;
  }

  // changing cells sizes by 8
  if (g_input_manager->get_key_press(SDLK_q, SDLK_LSHIFT)) {
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

  if (g_input_manager->get_key_press(SDLK_e, SDLK_LSHIFT)) {
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
        convert_to_grid();
      }
    }
  }
  // the asset creator, just add a name check the position and hit the Export!
  ImGui::Separator();
  if (m_selected_cell != nullptr) {
    ImGui::Text("Selected Cell");
    ImGuiUtils::header_input_text("Name", &m_selected_cell->name);
    ImGui::Text("X (%d)", m_selected_cell->get_x(start_pos.x));
    ImGui::Text("Y (%d)", m_selected_cell->get_y(start_pos.y));

    // create or add it to the .json of the assets
    if (ImGui::Button("Export")) {
      if (m_selected_cell->name != "") {
        nlohmann::json asset_json;
        asset_json["name"] = m_selected_cell->name;
        asset_json["palette"] = m_current_palette;
        asset_json["x"] = m_selected_cell->get_x(start_pos.x);
        asset_json["y"] = m_selected_cell->get_y(start_pos.y);
        asset_json["width"] = m_selected_cell->grid.w / 2;
        asset_json["height"] = m_selected_cell->grid.h / 2;

        std::string json_file_path =
            g_fini->get_value<std::string>("editor", "project_folder") +
            "\\res\\sprites\\sprites.json";

        if (FUtils::file_exists(json_file_path)) {
          std::ifstream file(json_file_path);
          nlohmann::json existing_json;
          file >> existing_json;
          file.close();
          existing_json.push_back(asset_json);
          std::ofstream out_file(json_file_path);
          out_file << existing_json.dump(4);
          out_file.close();
        } else {
          std::ofstream out_file(json_file_path);
          nlohmann::json new_json = {asset_json};
          out_file << new_json.dump(4);
          out_file.close();
        }

        m_is_dirty = true;
      } else {
        Logger::log("Please enter a name for the cell.");
      }
    }
  } else {
  }
  // sprite collection of the current pallete
  ImGui::Separator();
  if (m_current_palette != "") {
    for (const auto &data : m_grid_data) {
      if (data.pallete == m_current_palette) {
        ImGui::Text("Name: %s", data.name.c_str());
      }
    }
  } else {
  }
}

// FIX: this will be later converted to a res function to load the sprites from
// the data at least the idea
void Pallete::convert_to_grid() {
  m_cells_saved.clear();
  // convert the current image to a grid
  if (m_current_palette != "") {
    for (auto &cell : m_grid_data) {
      if (cell.pallete == m_current_palette) {
        GridCell grid_cell;
        grid_cell.name = cell.name;
        grid_cell.pallete = cell.pallete;
        grid_cell.grid.x =
            start_pos.x + cell.x * cell.w * g_camera->get_game_scale();
        grid_cell.grid.y =
            start_pos.y + cell.y * cell.h * g_camera->get_game_scale();
        grid_cell.grid.w = cell.w * g_camera->get_game_scale();
        grid_cell.grid.h = cell.h * g_camera->get_game_scale();
        grid_cell.color = Col(0, 255, 0, 25);
        grid_cell.m_selected_color = Col(255, 0, 0, 100);

        m_cells_saved[{cell.x, cell.y}] = grid_cell;
      }
    }
  }
}

void Pallete::draw() {
  if (m_current_palette != "") {

    if (m_current_image != nullptr) {

      g_renderer->draw_raw_sheet(m_current_image, start_pos);

      for (auto &cell : m_cells) {
        if (Mouse::is_at_area(cell.second.grid,
                              base_px_w * zoom * g_camera->get_game_scale(),
                              base_px_w * zoom * g_camera->get_game_scale())) {
          Rect cell_rect = cell.second.grid;
          g_renderer->draw_rect(cell.second.grid, cell.second.m_selected_color,
                                true);

          // click detection, open a popup or just at the side
          if (g_left_click) {
            m_selected_cell = &cell.second;
          }
        } else {
          g_renderer->draw_rect(cell.second.grid, cell.second.color, true);
        }
      }

      // drawing the saved cells to give a feedback
      for (const auto &cell : m_cells_saved) {
        if (m_current_palette != cell.second.pallete) {
          continue;
        }

        if (Mouse::is_at_area(cell.second.grid,
                              base_px_w * zoom * g_camera->get_game_scale(),
                              base_px_w * zoom * g_camera->get_game_scale())) {
          Rect cell_rect = cell.second.grid;
          g_renderer->draw_rect(cell.second.grid, cell.second.m_selected_color,
                                true);

          // click detection, but in this case it will trigger a warning??
          if (g_left_click) {
          }
        } else {
          g_renderer->draw_rect(cell.second.grid, cell.second.color, true);
        }
      }

    } else {
    }
  }
}

void Pallete::clean() { Logger::log("Tabs cleaned"); }
