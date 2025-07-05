#include "Prefab.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
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
            Logger::log(var_name);
            var_name = std::regex_replace(var_name, std::regex(";"),
                                          ""); // Trim trailing spaces
            Component var;
            var.type = var_name;
            var.val[0] = '\0';
            var.val_1[0] = '\0';
            var.val_2[0] = '\0';

            data.variables.push_back({var_type, var});
          }
        }
      }

      m_components[data.name] = data;
    }
  }
}

void Prefab::update() {
  // Update logic for tabs can be added here
}

void Prefab::side_draw() {
  if (ImGui::Button("Add New")) {
    PrefabData newPrefab;
    newPrefab.name = "New Prefab";
    m_prefabs[newPrefab.name] = newPrefab;
  }

  ImGui::Separator();

  for (auto &pair : m_prefabs) {
    auto &data = pair.second;
    if (ImGui::Button(data.name.c_str())) {

      m_current_prefab = &data;
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
      m_current_prefab = nullptr;
      m_prefabs.erase(data.name);
    }
  }
}

void Prefab::draw() {
  if (m_current_prefab != nullptr) {
    ImGuiUtils::header_input_text("Prefab Name", &m_current_prefab->name);

    for (auto &component : m_current_prefab->components) {
      ImGui::Text("Component: %s", component.name.c_str());
      for (const auto &var : component.variables) {
        ImGui::Text("Type: %s, Name: %s", var.first.c_str(),
                    var.second.type.c_str());
      }
      if (ImGui::Button(("Remove Component##" + component.name).c_str())) {
      }
    }

    ImGui::Separator();

    // dropdown list with all the components
    static int selected_component = -1;
    std::vector<std::string> component_names;
    for (const auto &pair : m_components) {
      component_names.push_back(pair.first);
    }
    if (ImGui::BeginCombo("Components",
                          selected_component == -1
                              ? "Select Component"
                              : component_names[selected_component].c_str())) {
      for (int i = 0; i < component_names.size(); ++i) {
        bool is_selected = (selected_component == i);
        if (ImGui::Selectable(component_names[i].c_str(), is_selected)) {
          selected_component = i;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("Add Component") && selected_component != -1) {
      std::string component_name = component_names[selected_component];
      if (m_components.find(component_name) != m_components.end()) {
        ComponentData &comp_data = m_components[component_name];
        m_current_prefab->components.push_back(comp_data);
      }
    }
  } else {
    ImGui::Text("No prefab selected.");
  }
}

void Prefab::clean() { Logger::log("Tabs cleaned"); }
