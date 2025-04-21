#include "PrefabEditor.hpp"

#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../tools/Logger.hpp"
#include "../../imgui/imgui_impl_opengl3.h"
#include <iostream>

PrefabEditor::PrefabEditor() {
}

void PrefabEditor::open() {
}

void PrefabEditor::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );


  ImGui::PopStyleColor();
  ImGui::End();
}

void PrefabEditor::update(){

}

void PrefabEditor::dispose() {
}

void PrefabEditor::draw() {
}

void PrefabEditor::reload() {
  Logger::log("Reloading Prefab Editor");
}
