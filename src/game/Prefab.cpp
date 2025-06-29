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

void Prefab::draw() { ImGui::Text("Hello Prefab!"); }

void Prefab::clean() { Logger::log("Tabs cleaned"); }
