#include "Editor.hpp"
#include "../core/global.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "Fini.hpp"

std::string project_folder;
std::string buffer_project_folder;

void Editor::init() {
  g_fini->initialize_value("editor", "project_folder",
                           std::string("res/editor/"));
  project_folder = g_fini->get_value<std::string>("editor", "project_folder");
  buffer_project_folder = project_folder;
}

void Editor::update() {}

void Editor::draw() {
  ImGuiUtils::header_input_text("Project Folder", &buffer_project_folder);
  if (buffer_project_folder != project_folder) {
    Logger::log("Project folder changed to: " + buffer_project_folder);
    g_fini->set_value("editor", "project_folder", buffer_project_folder);
    project_folder = buffer_project_folder;
  }
}

void Editor::clean() {}
