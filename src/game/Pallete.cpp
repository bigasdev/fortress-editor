#include "Pallete.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"

void Pallete::init() { Logger::log("Tabs initialized"); }

void Pallete::update() {}

void Pallete::side_draw() {
  ImGui::Text("This is the side panel for the pallete.");
}

void Pallete::draw() { ImGui::Text("Hello Pallete!"); }

void Pallete::clean() { Logger::log("Tabs cleaned"); }
