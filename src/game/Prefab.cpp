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

  std::string data_json_file_path =
      g_fini->get_value<std::string>("editor", "project_folder") +
      "\\res\\data\\" + "data.json";
  if (data_json_file_path.empty()) {
    Logger::log("Data JSON file path is empty.");
    return;
  }

  if (FUtils::file_exists(data_json_file_path)) {
    std::ifstream file(data_json_file_path);
    nlohmann::json existing_json;
    file >> existing_json;
    file.close();

    if (existing_json.contains("folders")) {
      for (const auto &folder : existing_json["folders"]) {
        m_folders.push_back(folder);
      }
    }

    if (existing_json.contains("data")) {
      for (const auto &item : existing_json["data"]) {
        Item new_item;
        new_item.name = item["name"];
        new_item.folder = item["folder"];
        new_item.sprite.name = item["sprite"]["name"];
        new_item.sprite.pallete = item["sprite"]["pallete"];

        if (item.contains("params")) {
          for (const auto &param : item["params"]) {
            std::string type = param["type"];
            if (type == "int") {
              auto p = std::make_unique<ItemParamInt>();
              p->name = param["name"];
              p->value = param["value"];
              new_item.params.push_back(std::move(p));
            } else if (type == "float") {
              auto p = std::make_unique<ItemParamFloat>();
              p->name = param["name"];
              p->value = param["value"];
              new_item.params.push_back(std::move(p));
            } else if (type == "string") {
              auto p = std::make_unique<ItemParamString>();
              p->name = param["name"];
              p->value = param["value"];
              new_item.params.push_back(std::move(p));
            } else if (type == "bool") {
              auto p = std::make_unique<ItemParamBool>();
              p->name = param["name"];
              p->value = param["value"];
              new_item.params.push_back(std::move(p));
            } else if (type == "vec2") {
              auto p = std::make_unique<ItemParamVec2>();
              p->name = param["name"];
              p->value.x = param["value"][0];
              p->value.y = param["value"][1];
              new_item.params.push_back(std::move(p));
            }
          }

          Logger::log_group("Item", new_item.name + " loaded with " +
                                        std::to_string(new_item.params.size()) +
                                        " params");
        }

        // TODO: REFACTOR THE FOLDERS AND OPENED ITEMS TO BE ENGINE-SIDE
        // OR AT LEAST CHECK IF THIS IS REALLY NEEDED

        m_items[new_item.name] = new_item;
        if (existing_json.contains("open_items") &&
            std::find(existing_json["open_items"].begin(),
                      existing_json["open_items"].end(),
                      new_item.name) != existing_json["open_items"].end()) {
          m_items_open[new_item.name] = true;
        } else {
          m_items_open[new_item.name] = false;
        }
      }
    }
  }
}

void Prefab::update() {
  if (g_input_manager->get_key_press(SDLK_d, SDLK_LCTRL)) {
    Logger::log_group("Item", "Open new item popup");
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
        Logger::error(
            "Item name is empty, can't create an item without a name");
        return;
      }

      if (m_items.find(item.name) != m_items.end()) {
        Logger::warn("Item with name " + item.name +
                     " already exists, can't create an item with the same "
                     "name");
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
        save();
      } else {
        Logger::warn("Folder already exists");
      }
    } else {
      Logger::warn("Folder name is empty");
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
  std::string json_file_path =
      g_fini->get_value<std::string>("editor", "project_folder") +
      "\\res\\data\\" + "data.json";

  if (json_file_path.empty()) {
    Logger::error("Prefab JSON file path is empty.");
    return;
  }

  nlohmann::json folders_json;
  for (auto folder : m_folders) {
    folders_json["folders"].push_back(folder);
  }

  nlohmann::json cache_json;
  for (auto open : m_items_open) {
    if (open.second) {
      cache_json["open_items"].push_back(open.first);
    }
  }

  nlohmann::json data_json;
  for (const auto &item : m_items) {
    nlohmann::json item_json;
    item_json["name"] = item.second.name;
    item_json["folder"] = item.second.folder;
    item_json["sprite"]["name"] = item.second.sprite.name;
    item_json["sprite"]["pallete"] = item.second.sprite.pallete;

    nlohmann::json param_json;
    for (auto &param : item.second.params) {
      param_json["name"] = param->name;
      if (auto p = dynamic_cast<ItemParamInt *>(param.get())) {
        param_json["type"] = "int";
        param_json["value"] = p->value;
      } else if (auto p = dynamic_cast<ItemParamFloat *>(param.get())) {
        param_json["type"] = "float";
        param_json["value"] = p->value;
      } else if (auto p = dynamic_cast<ItemParamString *>(param.get())) {
        param_json["type"] = "string";
        param_json["value"] = p->value;
      } else if (auto p = dynamic_cast<ItemParamBool *>(param.get())) {
        param_json["type"] = "bool";
        param_json["value"] = p->value;
      } else if (auto p = dynamic_cast<ItemParamVec2 *>(param.get())) {
        param_json["type"] = "vec2";
        param_json["value"] = {p->value.x, p->value.y};
      }
      item_json["params"].push_back(param_json);
    }

    data_json.push_back(item_json);

    Logger::log_group("Item", item.first + " saved with " +
                                  std::to_string(item.second.params.size()) +
                                  " params");
  }

  if (FUtils::file_exists(json_file_path)) {
    std::ifstream file(json_file_path);
    nlohmann::json existing_json;
    file >> existing_json;
    file.close();

    existing_json["folders"] = folders_json["folders"];
    existing_json["data"] = data_json;
    existing_json["open_items"] = cache_json["open_items"];

    std::ofstream out_file(json_file_path);
    out_file << existing_json.dump(4);
    out_file.close();
  } else {
    std::ofstream out_file(json_file_path);
    nlohmann::json new_json;
    new_json["folders"] = folders_json["folders"];
    new_json["data"] = data_json;
    new_json["open_items"] = cache_json["open_items"];
    out_file << new_json.dump(4);
    out_file.close();
  }
}

void Prefab::draw() {

  // items windows

  int id = 0;
  for (auto i : m_items_open) {
    if (i.second) {
      ImGui::Begin(("Item: " + i.first).c_str());
      ImGuiUtils::header_input_text("Data name:", &m_items[i.first].name);
      ImGuiUtils::header_input_text("Data folder:", &m_items[i.first].folder);
      int selected_sprite = -1;
      if (ImGui::BeginCombo(("Sprite##" + std::to_string(id++)).c_str(),
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
      if (selected_sprite >= 0 && selected_sprite < m_grid_data.size()) {
        m_items[i.first].sprite.name = m_grid_data[selected_sprite].name;
        m_items[i.first].sprite.pallete = m_grid_data[selected_sprite].pallete;
      }
      ImGui::Text("Sprite: %s", m_items[i.first].sprite.name.c_str());
      ImGui::Separator();
      if (ImGui::CollapsingHeader("Parameters")) {
        if (ImGui::Button("Add Int")) {
          m_items[i.first].params.push_back(std::make_unique<ItemParamInt>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Float")) {
          m_items[i.first].params.push_back(std::make_unique<ItemParamFloat>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Add String")) {
          m_items[i.first].params.push_back(
              std::make_unique<ItemParamString>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Bool")) {
          m_items[i.first].params.push_back(std::make_unique<ItemParamBool>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Vec2")) {
          m_items[i.first].params.push_back(std::make_unique<ItemParamVec2>());
        }

        int param_id = 0;
        for (auto &param : m_items[i.first].params) {
          param_id++;
          ImGui::PushID((i.first + std::to_string(param_id)).c_str());
          ImGuiUtils::header_input_text("Param Name", &param->name);
          if (auto p = dynamic_cast<ItemParamInt *>(param.get())) {
            ImGuiUtils::header_input_int("Int", &p->value);
          } else if (auto p = dynamic_cast<ItemParamFloat *>(param.get())) {
            ImGuiUtils::header_input_float("Float", &p->value);
          } else if (auto p = dynamic_cast<ItemParamString *>(param.get())) {
            ImGuiUtils::header_input_text("String", &p->value);
          } else if (auto p = dynamic_cast<ItemParamBool *>(param.get())) {
            // ImGui::Checkbox("Bool", &p->value);
          } else if (auto p = dynamic_cast<ItemParamVec2 *>(param.get())) {
            // ImGuiUtils::header_input_vec2("Vec2", &p->value);
          }

          if (ImGui::Button("Remove")) {
            m_items[i.first].params.erase(
                std::remove_if(m_items[i.first].params.begin(),
                               m_items[i.first].params.end(),
                               [&](const std::shared_ptr<ItemParam> &ptr) {
                                 return ptr->name == param->name;
                               }),
                m_items[i.first].params.end());
            ImGui::PopID();
            break;
          }
          ImGui::PopID();
        }
      }
      ImGui::Separator();

      // check if an imgui keyboard action was done
      if (ImGui::IsWindowFocused() &&
          ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)) &&
          (ImGui::GetIO().KeyCtrl)) {
        save();
      }

      ImGui::End();
    }
  }
}

void Prefab::clean() { Logger::log("Tabs cleaned"); }
