#include "AnimatorView.hpp"

#include <iostream>
#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../res/Res.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/ImGuiHelper.hpp"
#include "../../renderer/Sprite.hpp"
#include "../../imgui/imgui_impl_opengl3.h"
#include "SDL_gpu.h"


AnimatorView::AnimatorView() {
  m_sprites = g_res->get_sprites();
  m_selected_sprite = nullptr;
}

void AnimatorView::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar);

  //
  assets_child();
  animator_child();

  ImGui::PopStyleColor();
  ImGui::End();
}

void AnimatorView::assets_child() {
  if(m_sprites.empty()){
    return;
  }

  ImGui::BeginChild(" Assets", ImVec2(300, g_engine->get_window_size()->y - 35), true);
  //change to a real search later
  ImGui::InputText("##Search", "Search...", IM_ARRAYSIZE("Search"));
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(26, 20))) {
  }

  for(auto& asset : m_sprites){ 
    auto handle = GPU_GetTextureHandle(*g_res->get_texture(asset.second.sheet));
    
    int sprite_x = asset.second.dst_x * asset.second.wid;
    int sprite_y = asset.second.dst_y * asset.second.hei;
    int sprite_width = asset.second.wid;
    int sprite_height = asset.second.hei;
    int atlas_size = 500;

    ImVec2 uv0 = ImVec2((float)sprite_x / atlas_size, (float)sprite_y / atlas_size);

    ImVec2 uv1 = ImVec2((float)(sprite_x + sprite_width) / atlas_size,
                        (float)(sprite_y + sprite_height) / atlas_size);


    if (ImGui::ImageButton(asset.first.c_str(), (void *)(intptr_t)handle, ImVec2(32, 32), uv0, uv1)) {
      m_selected_sprite = &asset.second;
    }
    ImGui::Text(asset.first.c_str());
  }
  ImGui::EndChild();
}

void AnimatorView::animator_child() {
  if(m_selected_sprite == nullptr){
    return;
  }
  ImGui::SetNextWindowPos(ImVec2(395, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(g_engine->get_window_size()->x - 420, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin("@ Animator", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar);

  //this could be refactored later
  int frame_size_x = 100;
  int frame_size_y = 100;
  
  //thils will be a loop of the animations later 
  ImGui::BeginChild("Animation 1", ImVec2(ImGui::GetContentRegionAvail().x, 120), true);
  ImGui::BeginChild("Sprite", ImVec2(frame_size_x, frame_size_y), true);
  ImGui::Image((void *)(intptr_t)GPU_GetTextureHandle(*g_res->get_texture(m_selected_sprite->sheet)), ImVec2(32, 32));
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("Sprite Info", ImVec2(frame_size_x, frame_size_y), true);
  ImGui::Text(m_selected_sprite->sheet.c_str());
  ImGui::Text("Size: %dx%d", m_selected_sprite->wid, m_selected_sprite->hei);
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("Sprite Position X", ImVec2(frame_size_x, frame_size_y), true);
  ImGui::Text("X");
  ImGui::InputFloat("##X", &m_selected_sprite->dst_x);
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("Sprite Position Y", ImVec2(frame_size_x, frame_size_y), true);
  ImGui::Text("Y");
  ImGui::InputFloat("##Y", &m_selected_sprite->dst_y);
  ImGui::EndChild();
  ImGui::SameLine();
  /*ImGui::InputInt("Frames", nullptr);
  ImGui::SameLine();
  ImGui::Checkbox("Loop", nullptr);
  ImGui::SameLine();
  ImGui::Checkbox("Block Transition", nullptr);*/
  ImGui::EndChild();

  ImGui::SetNextWindowPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetContentRegionAvail().y + 10));
  ImGui::BeginChild("Create animation", ImVec2(ImGui::GetContentRegionAvail().x - 10, 120), true);
  
  static char animation_name[128] = "";
  ImGui::InputText("Animation Name", animation_name, IM_ARRAYSIZE(animation_name));
  if(ImGui::Button("Create", ImVec2(ImGui::GetContentRegionAvail().x - 10, 30))){
  }

  ImGui::EndChild();

  ImGui::PopStyleColor();
  ImGui::End();
}

void AnimatorView::update() {
}

void AnimatorView::dispose() {}

void AnimatorView::draw() {}
