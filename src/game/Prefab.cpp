#include "Prefab.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../renderer/Renderer.hpp"
#include "../res/Res.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "../tools/Mouse.hpp"
#include "DataLoader.hpp"
#include "Fini.hpp"
#include "Pallete.hpp"
#include "json.hpp"
#include <regex>

void Prefab::init() {
  m_grid_data.clear();

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

  // loading all the components from the project folder
  std::string components_file_path =
      g_fini->get_value<std::string>("editor", "project_folder") +
      "/src/components/";
  if (FUtils::folder_exists(components_file_path)) {
    auto components = Data_Loader::get_files(components_file_path, ".hpp");

    for (auto &component : components) {
      ComponentData data;
      auto name = component.substr(0, component.find_last_of('.'));
      data.name = name;

      if (data.name == "ComponentStore" || data.name == "IComponent")
        continue;
      if (data.name == "SpriteComponent" || data.name == "TransformComponent") {
      }

      std::ifstream file(components_file_path + component);
      std::string line;
      while (std::getline(file, line)) {
        line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");

        if (line.empty() || line.find("#") == 0 || line.find("{") == 0 ||
            line.find("}") == 0) {
          continue;
        }

        if (line.find("[[NotSerializable]]") != std::string::npos ||
            line.find("void") != std::string::npos ||
            line.find("this->name") != std::string::npos)
          continue;

        if (line.find(";") != std::string::npos &&
            line.find("(") == std::string::npos) {
          auto type_end = line.find(' ');
          auto var_start = line.find_last_of(' ', line.find(';')) + 1;
          if (type_end != std::string::npos && var_start != std::string::npos) {
            std::string var_type = line.substr(0, type_end);
            Logger::log(var_type);
            std::string var_name =
                line.substr(type_end + 1, line.find('=') - (type_end + 1) - 1);
            var_name = std::regex_replace(var_name, std::regex(";"),
                                          ""); // Trim trailing spaces
            Logger::log(var_name);

            auto default_value = line.substr(
                var_start, line.find(';') - var_start); // Get the default value
            Component var;
            var.type = var_type;
            var.value = var_name;
            if (var_type == "vec2") {
              var.val_1 = "0";
              var.val_2 = "0";
            } else {
              var.val_1 = default_value;
              var.val_2 = "";
            }

            data.variables.push_back({var_type, var});
          }
        }
      }

      m_components[data.name] = data;
    }
  }

  m_option_menu.options.clear();
  m_option_menu.options.push_back("Create Prefab");
}

void Prefab::update() {
  auto mouse_pos = Mouse::get_mouse_pos();
  if (mouse_pos.x > 167) {
    m_is_on_grid = true;
  } else {
    m_is_on_grid = false;
  }

  if (!m_is_on_grid) {
    return;
  }

  if (g_right_click) {
    Logger::log("Right click on prefab tab");
    m_option_menu.is_open = !m_option_menu.is_open;
    m_option_menu.pos = mouse_pos;
    g_right_click = false;
  }

  if (g_left_click) {
    if (m_option_menu.is_open) {
      if (Mouse::is_at_area({m_option_menu.pos.x, m_option_menu.pos.y, 96, 24},
                            16, 16)) {

        m_option_menu.is_open = false;
      }
    }
  }
}

void Prefab::side_draw() {
  Logger::log("Prefab side draw");
  ImGui::Begin("Hello World!");
  ImGui::Text("Hello World!");
  ImGui::End();
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
  // g_renderer->draw_text({170, 300}, mouse_pos_text.c_str(),
  // g_res->get_font("fusion10"), {255, 255, 255, 255}, 1);
  if (m_option_menu.is_open) {
    g_renderer->draw_rect({m_option_menu.pos.x, m_option_menu.pos.y, 96, 24},
                          {125, 125, 125, 120}, false);

    g_renderer->draw_text({m_option_menu.pos.x + 3, m_option_menu.pos.y + 3},
                          "Create Prefab", g_res->get_font("GalmuriMono9"),
                          {255, 255, 255, 255}, 1);
  }
}

void Prefab::clean() { Logger::log("Tabs cleaned"); }
