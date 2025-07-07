#include "Editor.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "Fini.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string project_folder;
std::string buffer_project_folder;
std::string components_folder;
std::string systems_folder;
std::string buffer_components_folder;
std::string buffer_systems_folder;

std::string component_name;

void Editor::init() {
  g_fini->initialize_value("editor", "project_folder",
                           std::string("res/editor/"));
  g_fini->initialize_value("editor", "components_folder",
                           std::string("src/components/"));
  g_fini->initialize_value("editor", "systems_folder",
                           std::string("src/systems/"));
  project_folder = g_fini->get_value<std::string>("editor", "project_folder");
  buffer_project_folder = project_folder;
  components_folder =
      g_fini->get_value<std::string>("editor", "components_folder");
  buffer_components_folder = components_folder;
  systems_folder = g_fini->get_value<std::string>("editor", "systems_folder");
  buffer_systems_folder = systems_folder;
}

void Editor::update() {}

void Editor::draw() {
  ImGuiUtils::header_input_text("Project Folder", &buffer_project_folder);
  if (buffer_project_folder != project_folder) {
    g_fini->set_value("editor", "project_folder", buffer_project_folder);
    project_folder = buffer_project_folder;
  }

  if (ImGui::CollapsingHeader("Component")) {
    ImGuiUtils::header_input_text("Components Folder",
                                  &buffer_components_folder);
    if (buffer_components_folder != components_folder) {
      g_fini->set_value("editor", "components_folder",
                        buffer_components_folder);
      components_folder = buffer_components_folder;
    }

    if (FUtils::folder_exists(components_folder)) {

      ImGui::Separator();

      ImGuiUtils::header_input_text("Component Name", &component_name);
      if (ImGui::Button("Create Component")) {
        if (!component_name.empty()) {
          std::ifstream file("res/templates/component_template.txt");

          if (!file.is_open()) {
            return;
          }

          std::stringstream buffer;
          buffer << file.rdbuf();
          auto templ = buffer.str();

          size_t pos;
          while ((pos = templ.find("{{CLASS_NAME}}")) != std::string::npos) {
            templ.replace(pos, std::string("{{CLASS_NAME}}").length(),
                          component_name);
          }
          std::ofstream out(components_folder + "/" + component_name + ".hpp");
          out << templ;
          out.close();
        }
      }
    }
  }

  if (ImGui::CollapsingHeader("System")) {
    ImGuiUtils::header_input_text("Systems Folder", &buffer_systems_folder);
    if (buffer_systems_folder != systems_folder) {
      g_fini->set_value("editor", "systems_folder", buffer_systems_folder);
      systems_folder = buffer_systems_folder;
    }
  }
}

void Editor::clean() {}
