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
}

void Prefab::update() {}

void Prefab::side_draw() { ImGui::Text("Hello World!"); }

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

void Prefab::draw() {}

void Prefab::clean() { Logger::log("Tabs cleaned"); }
