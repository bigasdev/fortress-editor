#include "BaseTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"

void BaseTab::open() {
}

void BaseTab::show() {
  ImGui::BeginChild(m_name.c_str(), ImVec2(150, 50), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  ImGui::Text(m_name.c_str());
  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::CalcTextSize(m_name.c_str()).x + 10);
  ImGui::PushID(m_name.c_str());
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
