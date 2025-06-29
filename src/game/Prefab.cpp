#include "Prefab.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"

void Prefab::init() { Logger::log("Tabs initialized"); }

void Prefab::update() {
  // Update logic for tabs can be added here
}

void Prefab::side_draw() {
  ImGui::Text("This is the side panel for the prefab.");
}

void Prefab::draw() {}

void Prefab::clean() { Logger::log("Tabs cleaned"); }
