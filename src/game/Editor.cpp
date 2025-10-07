#include "Editor.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../res/ResGenerator.hpp"
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
std::string system_name;

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
  if (ImGui::CollapsingHeader(" Editor")) {
    ImGuiUtils::header_input_text(" Project Folder", &buffer_project_folder);
    if (buffer_project_folder != project_folder) {
      g_fini->set_value("editor", "project_folder", buffer_project_folder);
      project_folder = buffer_project_folder;
    }

    if (ImGui::Button("Generate Resources")) {
      res::generate_resources(project_folder);
    }
  }

  if (ImGui::CollapsingHeader(" Component")) {
    ImGuiUtils::header_input_text(" Components Folder",
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
          component_name += "Component";
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

  if (ImGui::CollapsingHeader(" System")) {
    ImGuiUtils::header_input_text(" Systems Folder", &buffer_systems_folder);
    if (buffer_systems_folder != systems_folder) {
      g_fini->set_value("editor", "systems_folder", buffer_systems_folder);
      systems_folder = buffer_systems_folder;
    }

    if (FUtils::folder_exists(systems_folder)) {

      ImGui::Separator();

      ImGuiUtils::header_input_text("System Name", &system_name);
      if (ImGui::Button("Create System")) {
        if (!system_name.empty()) {
          system_name += "System";
          std::ifstream file_hpp("res/templates/system_header_template.txt");
          std::ifstream file_cpp("res/templates/system_source_template.txt");

          if (!file_hpp.is_open() || !file_cpp.is_open()) {
            return;
          }

          std::stringstream buffer_hpp, buffer_cpp;
          buffer_hpp << file_hpp.rdbuf();
          buffer_cpp << file_cpp.rdbuf();
          auto templ_hpp = buffer_hpp.str();
          auto templ_cpp = buffer_cpp.str();

          size_t pos;
          while ((pos = templ_hpp.find("{{CLASS_NAME}}")) !=
                 std::string::npos) {
            templ_hpp.replace(pos, std::string("{{CLASS_NAME}}").length(),
                              system_name);
          }
          while ((pos = templ_cpp.find("{{CLASS_NAME}}")) !=
                 std::string::npos) {
            templ_cpp.replace(pos, std::string("{{CLASS_NAME}}").length(),
                              system_name);
          }

          std::ofstream out_hpp(systems_folder + "/" + system_name + ".hpp");
          out_hpp << templ_hpp;
          out_hpp.close();
          std::ofstream out_cpp(systems_folder + "/" + system_name + ".cpp");
          out_cpp << templ_cpp;
          out_cpp.close();
          Logger::log("System created: " + system_name);
        }
      }
    }
  }
}

void Editor::clean() {}
