#include "BaseTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"

BaseTab::BaseTab(const std::string& _name)  {
  name = _name;
}

void BaseTab::open() {
}

void BaseTab::show() {
  ImGui::BeginChild(name.c_str(), ImVec2(150, 50), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  ImGui::Text(name.c_str());
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::CalcTextSize(name.c_str()).x + 10);
  ImGui::PushID(name.c_str());
  if(ImGui::Button("")){
    is_open = !is_open;
  }
  ImGui::PopID();
  ImGui::EndChild();
}

void BaseTab::update() {
}

void BaseTab::dispose() {
}

void BaseTab::draw() {
}

void BaseTab::reload() {
}

void BaseTab::save() {
    is_dirty = false;
}
