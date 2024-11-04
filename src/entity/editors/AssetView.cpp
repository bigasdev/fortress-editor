#include "AssetView.hpp"
#include "../../core/Engine.hpp"
#include "../../core/UndoManager.hpp"
#include "../../core/global.hpp"
#include "../../core/EditorDataManager.hpp"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../../renderer/Renderer.hpp"
#include "../../renderer/Sprite.hpp"
#include "../../res/Res.hpp"
#include "../../tools/ImGuiHelper.hpp"
#include "../actions/AssetAction.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include "cute_aseprite.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include "../../tools/Logger.hpp"

#include "../data/EntityData.hpp"
#include "../editors/InfoBar.hpp"


std::unique_ptr<InfoBar> info_bar;
std::map<std::string, Pallete> m_sprites;
std::map<std::string, EntityData> m_entities;

std::string m_selected_pallete;
static char m_search_entity[256] = "";

// settings
int sprite_x = 8, sprite_y = 8;

AssetView::AssetView(std::map<std::string, Sprite> sprites) {
  info_bar = std::make_unique<InfoBar>();

  for (auto &[key, value] : sprites) {
    Pallete pallete;
    pallete.sprite = &value;
    pallete.ase = cute_aseprite_load_from_file(
        ("res/" + key + ".aseprite").c_str(), NULL);
    m_sprites[key] = pallete;
  }

  m_groups.push_back("default");
}

void AssetView::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320,
                                  g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar);

  info_bar->show();

  entities();
  atlas();
  pallete();

  ImGui::PopStyleColor();
  ImGui::End();
}

//
// Entity child view is where we have the groups of the entities
//
void AssetView::entities() {
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 430));
  ImGui::BeginChild("Entities", ImVec2(300, 150), true);
  ImGui::Text("  Entities");
  ImGui::BeginTabBar("Groups");
  if (ImGui::Button("", ImVec2(26, 20))) {
    ImGui::OpenPopup("New Group");
  }
  if (ImGui::BeginPopupModal("New Group")) {
    ImGui::Text("Create a new group");
    static char group_name[128] = "";
    ImGui::InputText("Group Name", group_name, IM_ARRAYSIZE(group_name));
    if (ImGui::Button("Create", ImVec2(120, 0)) || g_enter_pressed) {
      m_groups.push_back(group_name);
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
  ImGui::SameLine();
  ImGui::InputText("##Search", m_search_entity, IM_ARRAYSIZE(m_search_entity));
  ImGui::SameLine();
  for (auto group : m_groups) {
    if (ImGui::BeginTabItem(group.c_str())) {
      for (auto &[key, value] : m_entities) {
        if (value.group == group) {
          auto asset = *g_res->get_texture(value.pallete_name);
          auto x = asset->texture_w;
          auto y = asset->texture_h;
          ImGui::Image(
              (void *)(intptr_t)ImGuiHelper::convert_to_imgui(
                  *g_res->get_texture(value.pallete_name)),
              ImVec2(16, 16),
              ImVec2((float)value.sprite_pos.x / x,
                     (float)value.sprite_pos.y / y),
              ImVec2((float)(value.sprite_pos.x + value.sprite_size.x) /
                         x,
                     (float)(value.sprite_pos.y + value.sprite_size.y) /
                         y));
          ImGui::SameLine();
          if (ImGui::Button(key.c_str(), ImVec2(150, 18))) {
            g_selected_entity = &value;
          }
        }
      }
      ImGui::EndTabItem();
    }
  }
  ImGui::EndTabBar();
  ImGui::EndChild();
}

//
// Atlas child view is where the selected .aseprite will be divided in
// selectable sprites with the size of sprite_x and sprite_y
//
int spr_pos_x = 0;
int spr_pos_y = 0;
void AssetView::atlas() {
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 270));
  ImGui::BeginChild("Atlas", ImVec2(300, 150), true);
  ImGui::Text("  Atlas");
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(24, 20))) {
    ImGui::OpenPopup("Settings");
  }
  if (ImGui::BeginPopup("Settings")) {
    ImGui::InputInt("Sprite X", &sprite_x);
    ImGui::InputInt("Sprite Y", &sprite_y);
    ImGui::EndPopup();
  }
  if (m_selected_pallete != "") {
    auto asset = *g_res->get_texture(m_selected_pallete);
    auto x = asset->texture_w;
    auto y = asset->texture_h;
    auto row = 0;

    for (int j = 0; j < y; j += sprite_x) {
      for (int i = 0; i < x; i += sprite_y) {
        if (g_renderer->is_rect_fully_transparent(
                asset, {i, j, sprite_x, sprite_y},
                m_sprites[m_selected_pallete].ase))
          continue;
        if (ImGui::ImageButton(
                ("t" + std::to_string(i) + std::to_string(j)).c_str(),
                (void *)(intptr_t)ImGuiHelper::convert_to_imgui(
                    *g_res->get_texture(m_selected_pallete)),
                ImVec2(48, 48), ImVec2((float)i / x, (float)j / y),
                ImVec2((float)(i + sprite_x) / x, (float)(j + sprite_y) / y))) {
          ImGui::OpenPopup("Create Entity");
          spr_pos_x = i;
          spr_pos_y = j;
        }
        ImGui::SameLine();
        row++;

        if (row == 5) {
          row = 0;
          ImGui::NewLine();
        }
      }
    }

    if (ImGui::BeginPopupModal("Create Entity")) {
      ImGui::Text("Create a new entity");
      static char entity_name[128] = "";
      ImGui::InputText("Entity Name", entity_name, IM_ARRAYSIZE(entity_name));
      if (ImGui::Button("Create", ImVec2(120, 0)) || g_enter_pressed) {
        EntityData data;
        data.name = entity_name;
        data.pallete_name = m_selected_pallete;
        data.group = "default";
        data.sprite_size = {sprite_x, sprite_y};
        data.sprite_pos = {spr_pos_x, spr_pos_y};
        data.atlas_pos = {spr_pos_x / sprite_x, spr_pos_y / sprite_y};
        auto action = new AssetAction(data, m_entities);
        g_undo_manager->add(action);
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }
  ImGui::EndChild();
}

//
// Pallete child view, its where all the .aseprites of the project will be shown
// to selection
//
void AssetView::pallete() {
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 110));
  ImGui::BeginChild(" Pallete", ImVec2(300, 100), true);
  ImGui::Text(" Pallete");
  ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.0f));
  for (auto &[key, value] : m_sprites) {
    if (m_selected_pallete == key) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.1, 0.45, 1.0));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0);
    }

    if (ImGui::Button((" " + key).c_str(), ImVec2(150, 18))) {
      m_selected_pallete = key;
      break;
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
      ImGui::BeginTooltip();
      ImGui::Image((void *)(intptr_t)ImGuiHelper::convert_to_imgui(
                       *g_res->get_texture(key)),
                   ImVec2(400, 400));
      ImGui::EndTooltip();
    }

    if (m_selected_pallete == key) {
      ImGui::PopStyleColor();
      ImGui::PopStyleVar();
    }
  }
  ImGui::PopStyleVar();
  ImGui::EndChild();
}

void AssetView::update() {
  if(g_ctrl_pressed && g_s_pressed){
    g_editor_data_manager->export_(m_entities);
  }
  if(g_ctrl_pressed && g_o_pressed){
    g_editor_data_manager->import(m_entities);
    Logger::log("Assets from the asset view");
    for(auto &asset : m_entities){
      Logger::log(asset.first);
    }
  }
}

void AssetView::dispose() {
  for (auto &[key, value] : m_sprites) {
    cute_aseprite_free(value.ase);
  }
}
