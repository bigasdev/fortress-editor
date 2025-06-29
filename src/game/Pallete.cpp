#include "Pallete.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "Fini.hpp"

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

void Pallete::update() {}

void Pallete::side_draw() {
  ImGui::Text("This is the side panel for the pallete.");
  for (const auto &pallete : m_palettes) {
    if (FUtils::get_file_extension(pallete, ".aseprite")) {
      if (ImGui::Button((FUtils::get_file_name(pallete)).c_str())) {
      }
    }
  }
}

void Pallete::draw() { ImGui::Text("Hello Pallete!"); }

void Pallete::clean() { Logger::log("Tabs cleaned"); }
