#include "AnimatorView.hpp"

#include <iostream>
#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../imgui/imgui_impl_opengl3.h"


AnimatorView::AnimatorView() {}

void AnimatorView::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar);

  //
  assets_child();

  ImGui::PopStyleColor();
  ImGui::End();
}

void AnimatorView::assets_child() {
  ImGui::BeginChild(" Assets", ImVec2(300, g_engine->get_window_size()->y - 35), true);
  //change to a real search later
  ImGui::InputText("##Search", "Search...", IM_ARRAYSIZE("Search"));
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(26, 20))) {
  }
  ImGui::EndChild();
}

void AnimatorView::update() {}

void AnimatorView::dispose() {}

void AnimatorView::draw() {}
