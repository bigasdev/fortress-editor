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

}

void AssetInfo::update() {
}

void AssetInfo::dispose() {
}

void AssetInfo::draw() {
}


