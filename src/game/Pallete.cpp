#include "Pallete.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"

void Pallete::init() { Logger::log("Tabs initialized"); }

void Pallete::update() {}

void Pallete::draw() { ImGui::Text("Hello Pallete!"); }

void Pallete::clean() { Logger::log("Tabs cleaned"); }
