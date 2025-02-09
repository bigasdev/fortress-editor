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

  for(auto asset : m_sprites){ 
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

void AnimatorView::update() {
  if(m_selected_sprite != nullptr){
    Logger::log("test");
  }
}

void AnimatorView::dispose() {}

void AnimatorView::draw() {}
