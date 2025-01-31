#include "AssetInfo.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../data/EntityData.hpp"
#include "../../core/global.hpp"

AssetInfo::AssetInfo(EntityData* entity_data) : m_entity_data(entity_data) {}

AssetInfo::~AssetInfo() {}

void AssetInfo::show() {

  static char name[128] = "";
  ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
  ImGui::SameLine();
  if(ImGui::Button("Rename")) {
    g_selected_entity->name = name;
  }
  ImGui::InputInt("Size X", &g_selected_entity->sprite_size.x);
  ImGui::InputInt("Size Y", &g_selected_entity->sprite_size.y);
  ImGui::InputInt("Position X", &g_selected_entity->atlas_pos.x);
  ImGui::InputInt("Position Y", &g_selected_entity->atlas_pos.y);
  ImGui::InputInt("Collision Box X", &g_selected_entity->collision_box.x);
  ImGui::InputInt("Collision Box Y", &g_selected_entity->collision_box.y);
  ImGui::InputInt("Offset X", &g_selected_entity->sprite_offset.x);
  ImGui::InputInt("Offset Y", &g_selected_entity->sprite_offset.y);

  ImGui::Text(" Pivot");

  if(ImGui::Button("Top Left")) {
    g_selected_entity->sprite_offset = {0, 0};
  }
  ImGui::SameLine();
  if(ImGui::Button("Top Right")) {
    g_selected_entity->sprite_offset = {g_selected_entity->sprite_size.x, 0};
  }
  ImGui::SameLine();
  if(ImGui::Button("Bottom Left")) {
    g_selected_entity->sprite_offset = {0, g_selected_entity->sprite_size.y};
  }
  if(ImGui::Button("Bottom Right")) {
    g_selected_entity->sprite_offset = {g_selected_entity->sprite_size.x, g_selected_entity->sprite_size.y};
  }
  ImGui::SameLine();
  if(ImGui::Button("Center")) {
    g_selected_entity->sprite_offset = {g_selected_entity->sprite_size.x / 2, g_selected_entity->sprite_size.y / 2};
  }
  if(ImGui::Button("Bottom Center")) {
    g_selected_entity->sprite_offset = {g_selected_entity->sprite_size.x / 2, 0};
  }
}

void AssetInfo::update() {
}

void AssetInfo::dispose() {
}

void AssetInfo::draw() {
}


