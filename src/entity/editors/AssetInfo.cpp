#include "AssetInfo.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../data/EntityData.hpp"
#include "DataLoader.hpp"
#include "../../core/EditorDataManager.hpp"
#include "../../core/global.hpp"

AssetInfo::AssetInfo(EntityData* entity_data) : m_entity_data(entity_data) {}

AssetInfo::~AssetInfo() {}

void AssetInfo::open() {
}

void AssetInfo::show() {
  static char name[128] = "";
  ImGui::BeginChild("Asset Info", ImVec2(400,50), true);
  ImGui::Text(" Info");
  ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
  ImGui::SameLine();
  if(ImGui::Button("Rename")) {
    g_selected_entity->name = name;
  }
  ImGui::EndChild();

  ImGui::BeginChild("&Components", ImVec2(400, 200), true);
  ImGui::Text(" Components");
  //components section 
  for(auto &component : g_selected_entity->components) {
    if (ImGui::Checkbox(("##Active" + component.second.name).c_str(), &component.second.is_active)) {
    }
    ImGui::SameLine(); 
    if (ImGui::CollapsingHeader(component.second.name.c_str())) {
      ImGui::PushID(component.second.name.c_str());
      if (ImGui::Button("  Edit")) {
        auto command = "start cmd /k nvim " + g_folder_path + "/src/components/" + component.second.name + ".hpp";
        system(command.c_str());
      }
      ImGui::PopID();
      for(auto& var : component.second.variables) {
        if(var.first == "vec2"){
          ImGui::Text(var.second.name.c_str());
          ImGui::SameLine();
          ImGui::InputText(("##" + var.second.name).c_str(), var.second.val_1, IM_ARRAYSIZE(var.second.val_1));
          ImGui::SameLine();
          ImGui::InputText(("##1" + var.second.name).c_str(), var.second.val_2, IM_ARRAYSIZE(var.second.val_2));
          char formattedValue[128];
          std::snprintf(formattedValue, sizeof(formattedValue), "{%s, %s}", var.second.val_1, var.second.val_2);
          for (int i = 0; i < sizeof(var.second.val); ++i) {
            var.second.val[i] = formattedValue[i];
          }
          continue;
        }

        ImGui::Text(var.second.name.c_str());
        ImGui::SameLine();
        ImGui::InputText(("##" + var.second.name).c_str(), var.second.val, IM_ARRAYSIZE(var.second.val));
      }
    }
  }
  ImGui::EndChild();


  ImGui::BeginChild(" Transform", ImVec2(400, 180), true);
  ImGui::Text(" Transform");
  ImGui::PushItemWidth(100);
  ImGui::Text("Position");
  ImGui::SameLine(120);
  ImGui::Text("X");
  ImGui::SameLine();
  ImGui::InputInt("##Position X", &g_selected_entity->atlas_pos.x);
  ImGui::SameLine();
  ImGui::Text("Y");
  ImGui::SameLine();
  ImGui::InputInt("##Position Y", &g_selected_entity->atlas_pos.y);
  ImGui::Text("Scale");
  ImGui::SameLine(120);
  ImGui::Text("X");
  ImGui::SameLine();
  ImGui::InputInt("##Size X", &g_selected_entity->sprite_size.x);
  ImGui::SameLine();
  ImGui::Text("Y");
  ImGui::SameLine();
  ImGui::InputInt("##Size Y", &g_selected_entity->sprite_size.y);
  ImGui::Text("Sprite Offset");
  ImGui::SameLine(120);
  ImGui::Text("X");
  ImGui::SameLine();
  ImGui::InputInt("##Offset X", &g_selected_entity->sprite_offset.x);
  ImGui::SameLine();
  ImGui::Text("Y");
  ImGui::SameLine();
  ImGui::InputInt("##Offset Y", &g_selected_entity->sprite_offset.y);
  ImGui::PopItemWidth();

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


  ImGui::EndChild();

  ImGui::BeginChild(" Collision", ImVec2(400, 70), true);
  ImGui::Text(" Collision");
  ImGui::PushItemWidth(100);
  ImGui::Text("Size");
  ImGui::SameLine(120);
  ImGui::Text("X");
  ImGui::SameLine();
  ImGui::InputInt("##Collision Box X", &g_selected_entity->collision_box.x);
  ImGui::SameLine();
  ImGui::Text("Y");
  ImGui::SameLine();
  ImGui::InputInt("##Collision Box Y", &g_selected_entity->collision_box.y);
  ImGui::Text("Offset");
  ImGui::SameLine(120);
  ImGui::Text("X");
  ImGui::SameLine();
  ImGui::InputInt("##Collision Offset X", &g_selected_entity->collision_offset.x);
  ImGui::SameLine();
  ImGui::Text("Y");
  ImGui::SameLine();
  ImGui::InputInt("##Collision Offset Y", &g_selected_entity->collision_offset.y);
  ImGui::PopItemWidth();
  ImGui::EndChild();
}

void AssetInfo::update() {
}

void AssetInfo::dispose() {
}

void AssetInfo::draw() {
}

void AssetInfo::reload() {
}


