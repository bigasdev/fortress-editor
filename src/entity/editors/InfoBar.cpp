#include "InfoBar.hpp"
#include "../../imgui/imgui.h"
#include "../../tools/Mouse.hpp"
#include "../../core/global.hpp"
#include "../../core/Engine.hpp"
#include ".././data/EntityData.hpp"

InfoBar::InfoBar() {}

void InfoBar::show() {
  ImGui::SetNextWindowPos(ImVec2(400, g_engine->get_window_size()->y - 50));
  ImGui::SetNextWindowSize(ImVec2(800, 50));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1, 0.1, 0.1, 1.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.62, 0.36, 0.95, 1.0));
  ImGui::Begin("Info Bar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
  ImGui::BeginChild("Sprite size", ImVec2(150, 30), true);
  if(g_selected_entity != nullptr){
  ImGui::Text("Sprite size: %d, %d",
              static_cast<int>(g_selected_entity->sprite_size.x),
              static_cast<int>(g_selected_entity->sprite_size.y));
  }
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("Sprite collision box", ImVec2(210, 30), true);

  if(g_selected_entity != nullptr){
  ImGui::Text("Sprite Collision box: %d, %d",
              static_cast<int>(g_selected_entity->collision_box.x),
              static_cast<int>(g_selected_entity->collision_box.y));
  }
  ImGui::EndChild();
  ImGui::SameLine();
  //Placeholder for now, later this will be able to be changed so we remove the 16 magic number
  ImGui::BeginChild("Grid", ImVec2(140, 30), true);
  ImGui::Text("Grid size: %d, %d",
              16,
              16);
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("Cursor Position", ImVec2(200, 30), true);
  ImGui::Text("Cursor Position: %d, %d",
              static_cast<int>(Mouse::get_mouse_pos().x),
              static_cast<int>(Mouse::get_mouse_pos().y));
  ImGui::EndChild();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
  ImGui::PopStyleColor();
  ImGui::End();
}

void InfoBar::update() {}

void InfoBar::dispose() {}

void InfoBar::draw() {}
