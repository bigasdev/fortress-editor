#include "Editor.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"

void Editor::init() { Logger::log("Tabs initialized"); }

void Editor::update() {
  // Update logic for tabs can be added here
}

void Editor::draw() { ImGui::Text("Hello Editor!"); }

void Editor::clean() {}
