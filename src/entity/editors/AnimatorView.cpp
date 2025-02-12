#include "AnimatorView.hpp"

#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../core/InputManager.hpp"
#include "../../core/EditorDataManager.hpp"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../../renderer/Sprite.hpp"
#include "../../res/Res.hpp"
#include "../../tools/ImGuiHelper.hpp"
#include "../../tools/Logger.hpp"
#include "../actions/AnimationAction.hpp"
#include "../../core/UndoManager.hpp"
#include "SDL_gpu.h"
#include <iostream>

AnimatorView::AnimatorView() {
  m_sprites = g_res->get_sprites();

  for (auto &sprite : m_sprites) {
    Animator anim;
    anim.sprite = &sprite.second;
    m_animators[sprite.first] = anim;
  }

  // Log test for the animators
  for (auto &anim : m_animators) {
    Logger::log("Animator: " + anim.first);
  }
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
  if (m_sprites.empty()) {
    return;
  }

  ImGui::BeginChild(" Assets",
                    ImVec2(300, g_engine->get_window_size()->y - 35), true);
  // change to a real search later
  ImGui::InputText("##Search", "Search...", IM_ARRAYSIZE("Search"));
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(26, 20))) {
  }

  for (auto &asset : m_sprites) {
    auto handle = GPU_GetTextureHandle(*g_res->get_texture(asset.second.sheet));

    int sprite_x = asset.second.dst_x * asset.second.wid;
    int sprite_y = asset.second.dst_y * asset.second.hei;
    int sprite_width = asset.second.wid;
    int sprite_height = asset.second.hei;
    int atlas_size = 500;

    ImVec2 uv0 =
        ImVec2((float)sprite_x / atlas_size, (float)sprite_y / atlas_size);

    ImVec2 uv1 = ImVec2((float)(sprite_x + sprite_width) / atlas_size,
                        (float)(sprite_y + sprite_height) / atlas_size);

    if (ImGui::ImageButton(asset.first.c_str(), (void *)(intptr_t)handle,
                           ImVec2(32, 32), uv0, uv1)) {

      m_selected_animator = &m_animators[asset.first];
      m_selected_animator->name = asset.first;
    }
    ImGui::Text(asset.first.c_str());
  }
  ImGui::EndChild();
}

void AnimatorView::animator_child() {
  if(m_selected_animator == nullptr){
    return;
  }
  if (m_selected_animator->sprite == nullptr) {
    return;
  }
  ImGui::SetNextWindowPos(ImVec2(395, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(g_engine->get_window_size()->x - 420,
                                  g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin("@ Animator", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar);

  // this could be refactored later
  int frame_size_x = 100;
  int frame_size_y = 100;

  for (auto &anim : m_selected_animator->animations) {
    ImGui::BeginChild("Animation 1",
                      ImVec2(ImGui::GetContentRegionAvail().x, 120), true);
    ImGui::BeginChild("Sprite", ImVec2(frame_size_x, frame_size_y), true);
    ImGui::Image((void *)(intptr_t)GPU_GetTextureHandle(
                     *g_res->get_texture(m_selected_animator->sprite->sheet)),
                 ImVec2(32, 32));
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite Info", ImVec2(frame_size_x, frame_size_y), true);
    ImGui::Text(anim.first.c_str());
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite Position X", ImVec2(frame_size_x, frame_size_y),
                      true);
    ImGui::Text("X");
    ImGui::InputInt("##X", &anim.second.x);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite Position Y", ImVec2(frame_size_x, frame_size_y),
                      true);
    ImGui::Text("Y");
    ImGui::InputInt("##Y", &anim.second.y);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::InputInt("Frames", &anim.second.frames);
    ImGui::SameLine();
    ImGui::Checkbox("Loop", &anim.second.loop);
    ImGui::SameLine();
    ImGui::Checkbox("Block Transition", &anim.second.block_transition);
    ImGui::EndChild();
  }

  // thils will be a loop of the animations later

  ImGui::SetNextWindowPos(ImVec2(ImGui::GetCursorScreenPos().x,
                                 ImGui::GetContentRegionAvail().y + 10));
  ImGui::BeginChild("Create animation",
                    ImVec2(ImGui::GetContentRegionAvail().x - 10, 120), true);

  static char animation_name[128] = "";
  ImGui::InputText("Animation Name", animation_name,
                   IM_ARRAYSIZE(animation_name));
  if (ImGui::Button("Create",
                    ImVec2(ImGui::GetContentRegionAvail().x - 10, 30))) {
    Animation anim;
    anim.parent = m_selected_animator->name;
    anim.name = animation_name;
    m_selected_animator->animations[animation_name] = anim;
    AnimationAction *action = new AnimationAction(anim, m_animators);
    g_undo_manager->add(action);
  }

  ImGui::EndChild();

  ImGui::PopStyleColor();
  ImGui::End();
}

void AnimatorView::update() {
  if(g_ctrl_pressed and g_s_pressed){
    g_editor_data_manager->export_animators(m_animators);
    g_s_pressed = false;
    g_ctrl_pressed = false;
  }
}

void AnimatorView::dispose() {}

void AnimatorView::draw() {}
