#include "FloatingButtons.hpp"

#include "../../imgui/imgui.h"

FloatingButtons::FloatingButtons() {
  m_buttons.push_back({"hide_grid", ""});
  m_buttons.push_back({"hide_cbox", ""});
}

void FloatingButtons::show() {
  ImGui::SetNextWindowPos(ImVec2(395, 20));
  ImGui::SetNextWindowSize(ImVec2(34, 66));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.62, 0.36, 0.95, 1.0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.42, 0.36, 0.95, 1.0));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20.0f, 2.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
  ImGui::Begin("Floating Buttons", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
  for (auto &button : m_buttons) {
    if (ImGui::Button(button.display.c_str(), ImVec2(30,30))) {
      button.is_pressed = !button.is_pressed;
    }
  }
  ImGui::PopStyleColor(2);
  ImGui::PopStyleVar(2);
  ImGui::End();
}

void FloatingButtons::update() {}

void FloatingButtons::dispose() {}

void FloatingButtons::draw() {}

