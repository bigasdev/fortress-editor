#include "GameProfileTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"

#include "../../tools/Logger.hpp"

GameProfileTab::GameProfileTab(const std::string& _name) {
  name = _name;
}

void GameProfileTab::open() {
}

void GameProfileTab::show() {
  ImGui::BeginChild(name.c_str(), ImVec2(150, 50), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  ImGui::Text(name.c_str());
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::CalcTextSize(name.c_str()).x + 10);
  ImGui::PushID(name.c_str());
  if (ImGui::Button("")) {
    is_open = !is_open;
  }
  ImGui::PopID();
  ImGui::EndChild();
}

void GameProfileTab::update() {
}

void GameProfileTab::dispose() {
}

void GameProfileTab::draw() {
}

void GameProfileTab::reload() {
}

void GameProfileTab::save() {
  is_dirty = false;
}
