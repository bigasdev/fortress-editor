#include "Prefab.hpp"
#include "../core/InputManager.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/Sprite.hpp"
#include "../res/Res.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "../tools/Mouse.hpp"
#include "DataLoader.hpp"
#include "Fini.hpp"
#include "Pallete.hpp"
#include "SDL_keycode.h"
#include "json.hpp"
#include <regex>

void Prefab::init() {
  m_grid_data.clear();
  m_items_open.clear();

  std::string json_file_path =
      g_fini->get_value<std::string>("editor", "project_folder") +
      "\\res\\sprites\\sprites.json";
  if (json_file_path.empty()) {
    Logger::log("Prefab JSON file path is empty.");
    return;
  }
  if (FUtils::file_exists(json_file_path)) {
    std::ifstream file(json_file_path);
    nlohmann::json existing_json;
    file >> existing_json;
    file.close();

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
}

void Prefab::update() {
  if (g_input_manager->get_key_press(SDLK_d, SDLK_LCTRL)) {
    std::cout << "Create Item Popup" << std::endl;
    m_create_item_popup = true;
  }
}

void Prefab::side_draw() {
  if (m_create_item_popup) {
    ImGui::OpenPopup("Create Item");
    m_create_item_popup = false;
  }

  if (ImGui::BeginPopupModal("Create Item", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    static char item_name[128] = "";
    ImGui::InputText("Item Name", item_name, IM_ARRAYSIZE(item_name));

    static int selected_folder = -1;
    if (ImGui::BeginCombo("Folder", selected_folder >= 0 &&
                                            selected_folder < m_folders.size()
                                        ? m_folders[selected_folder].c_str()
                                        : "Select Folder")) {
      for (int n = 0; n < m_folders.size(); n++) {
        bool is_selected = (selected_folder == n);
        if (ImGui::Selectable(m_folders[n].c_str(), is_selected)) {
          selected_folder = n;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    static int selected_sprite = -1;
    if (ImGui::BeginCombo("Sprite",
                          selected_sprite >= 0 &&
                                  selected_sprite < m_grid_data.size()
                              ? m_grid_data[selected_sprite].name.c_str()
                              : "Select Sprite")) {
      for (int n = 0; n < m_grid_data.size(); n++) {
        bool is_selected = (selected_sprite == n);
        if (ImGui::Selectable(m_grid_data[n].name.c_str(), is_selected)) {
          selected_sprite = n;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("Create", ImVec2(120, 0))) {
      Item item;
      item.name = item_name;
      if (selected_folder >= 0 && selected_folder < m_folders.size()) {
        item.folder = m_folders[selected_folder];
      } else {
        std::cout << "No folder selected, can't create an item without folder, "
                     "returning..."
                  << std::endl;
        return;
      }

      if (selected_sprite >= 0 && selected_sprite < m_grid_data.size()) {
        item.sprite.name = m_grid_data[selected_sprite].name;
        item.sprite.pallete = m_grid_data[selected_sprite].pallete;
      } else {
      }
      if (item.name.empty()) {
        std::cout << "Item name is empty, can't create an item without a name, "
                     "returning..."
                  << std::endl;
        return;
      }

      if (m_items.find(item.name) != m_items.end()) {
        std::cout << "Item with name " << item.name
                  << " already exists, can't create an item with the same "
                     "name, returning..."
                  << std::endl;
        return;
      }

      m_items[item.name] = item;
      m_items_open[item.name] = false;
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGuiUtils::header_input_text("Folder Name", &m_folder_name_cache);
  ImGui::SameLine();
  if (ImGui::Button("Add Folder")) {
    if (!m_folder_name_cache.empty()) {
      if (std::find(m_folders.begin(), m_folders.end(), m_folder_name_cache) ==
          m_folders.end()) {
        m_folders.push_back(m_folder_name_cache);
        m_folder_name_cache.clear();
      } else {
        Logger::log("Folder already exists");
      }
    } else {
      Logger::log("Folder name is empty");
    }
  }

  ImGui::Separator();

  for (auto f : m_folders) {
    if (ImGui::CollapsingHeader(f.c_str())) {
      for (const auto &item : m_items) {
        if (item.second.folder == f) {
          if (ImGui::Selectable(item.first.c_str())) {
            m_items_open[item.first] = !m_items_open[item.first];
          }
        }
      }
    }
  }
}

void Prefab::save() {
  /*std::string json_file_path =
      g_fini->get_value<std::string>("editor", "project_folder") +
      "\\res\\prefabs\\" + "prefabs.json";

  if (json_file_path.empty()) {
    Logger::log("Prefab JSON file path is empty.");
    return;
  }

  nlohmann::json prefab_json;
  for (const auto &prefab : m_prefabs) {
    nlohmann::json prefab_data;
    prefab_data["name"] = prefab.name;

    for (const auto &component : prefab.components) {
      nlohmann::json component_data;
      component_data["name"] = component.name;

      for (const auto &var : component.variables) {
        nlohmann::json variable_data;
        if (var.second.type == "vec2") {
          variable_data["value"] =
              "{" + var.second.val_1 + ", " + var.second.val_2 + "}";
        } else {
          variable_data["value"] = var.second.val_1;
        }

        component_data["variables"].push_back(variable_data);
      }

      prefab_data["components"].push_back(component_data);
    }

    prefab_json.push_back(prefab_data);
  }

  std::ofstream file(json_file_path);
  file << prefab_json.dump(4);
  file.close();*/
}

void Prefab::draw() {

  // items windows

  for (auto i : m_items_open) {
    if (i.second) {
      ImGui::BeginChild(("Item: " + i.first).c_str(), {0, 250});
      ImGui::Text(("Name: " + m_items[i.first].name).c_str());
      ImGui::Text(("Folder: " + m_items[i.first].folder).c_str());
      ImGui::Text(("Sprite Name: " + m_items[i.first].sprite.name).c_str());
      ImGui::Text(
          ("Sprite Pallete: " + m_items[i.first].sprite.pallete).c_str());
      ImGui::Separator();
      ImGui::Text("Parameters:");
      for (const auto &param : m_items[i.first].params) {
        ImGui::Text(param.first.c_str());
      }
      ImGui::EndChild();
    }
  }
}

void Prefab::clean() { Logger::log("Tabs cleaned"); }
