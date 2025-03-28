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
#include "../../core/Timer.hpp"
#include "Fini.hpp"

AnimatorView::AnimatorView() {
  m_sprites = g_res->get_sprites();

  for (auto &sprite : m_sprites) {
    Animator anim;
    anim.sprite = &sprite.second;
    m_animators[sprite.first] = anim;
  }

  auto anim_json = g_fini->get_value<std::string>("last", "animator");
  if(anim_json != ""){
    m_animators = g_editor_data_manager->import_animators(anim_json);
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
  static char entity_search[128] = "";
  ImGui::InputText("##Search", entity_search, IM_ARRAYSIZE(entity_search));
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(26, 20))) {
  }

  int idx = 1;

  for (auto &asset : m_sprites) {
    if(asset.first.find(entity_search) == std::string::npos){
      continue;
    }
    
    if (idx == 5) {
      ImGui::NewLine();
      idx = 1;
    }

    ImGui::BeginChild(asset.first.c_str(), ImVec2(64, 70), true);
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
      m_selected_animator->sprite = &asset.second;
      m_selected_animator->name = asset.first;
    }
    ImGui::Text(asset.first.c_str());
    ImGui::EndChild();
    if(idx > 0) ImGui::SameLine();
    idx++;
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
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

  // this could be refactored later
  int frame_size_x = 100;
  int frame_size_y = 100;

  for (auto& [name, animation] : m_selected_animator->animations) {
    auto handle = GPU_GetTextureHandle(*g_res->get_texture(m_selected_animator->sprite->sheet));

    int sprite_x = (m_selected_animator->sprite->dst_x + animation.x + animation.current_frame )* m_selected_animator->sprite->wid;
    int sprite_y = (m_selected_animator->sprite->dst_y + animation.y) * m_selected_animator->sprite->hei;
    int sprite_width = m_selected_animator->sprite->wid;
    int sprite_height = m_selected_animator->sprite->hei;
    int atlas_size = 500;

    ImVec2 uv0 =
        ImVec2((float)sprite_x / atlas_size, (float)sprite_y / atlas_size);

    ImVec2 uv1 = ImVec2((float)(sprite_x + sprite_width) / atlas_size,
                        (float)(sprite_y + sprite_height) / atlas_size);
    
    ImGui::BeginChild(name.c_str(),
                      ImVec2(ImGui::GetContentRegionAvail().x, 120), true);
    ImGui::BeginChild((name + "_Sprite").c_str(), ImVec2(frame_size_x, frame_size_y), true);
    ImGui::Image((void *)(intptr_t)handle,
                 ImVec2(64, 64), uv0, uv1);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite animation state", ImVec2(220, frame_size_y), true);
    ImGui::Text("Current Frame: %d", animation.current_frame);
    ImGui::Text("Current Timer: %f", animation.current_timer);
    if(ImGui::Button(("Play##" + animation.name).c_str())){
      animation.is_playing = !animation.is_playing;
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite Info", ImVec2(frame_size_x, frame_size_y), true);
    ImGui::Text(name.c_str());
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite Position X", ImVec2(frame_size_x, frame_size_y),
                      true);
    ImGui::Text("X");
    ImGui::InputInt(("X##" + animation.name).c_str(), &animation.x);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Sprite Position Y", ImVec2(frame_size_x, frame_size_y),
                      true);
    ImGui::Text("Y");
    ImGui::InputInt(("Y##" + animation.name).c_str(), &animation.y);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Frames", ImVec2(100, frame_size_y),
                      true);
    ImGui::InputInt(("Frames##" + animation.name).c_str(), &animation.frames);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Loop", ImVec2(70, frame_size_y),
                      true);
    ImGui::Checkbox(("Loop##" + animation.name).c_str(), &animation.loop);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Block_Transition", ImVec2(70, frame_size_y),
                      true);
    ImGui::Checkbox(("Block Transition##" + animation.name).c_str(), &animation.block_transition);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Delete", ImVec2(70, frame_size_y),
                      true);
    if(ImGui::Button(("Delete##" + animation.name).c_str())){
      animation.should_delete = true;
    }
    ImGui::EndChild();
    ImGui::EndChild();

  }

  // thils will be a loop of the animations later

  ImGui::SetNextWindowPos(ImVec2(ImGui::GetCursorScreenPos().x,
                                 ImGui::GetContentRegionMax().y - 40));
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
    anim.frames = 4;
    anim.x = 0;
    anim.y = 0;
    anim.current_frame = 0;
    anim.current_timer = 0;
    anim.is_playing = true;
    anim.should_delete = false;
    m_selected_animator->animations[animation_name] = anim;
    //AnimationAction *action = new AnimationAction(anim, m_animators);
    //g_undo_manager->add(action);
  }

  ImGui::EndChild();

  ImGui::PopStyleColor();
  ImGui::End();
}

void AnimatorView::update() {
  if(g_ctrl_pressed and g_s_pressed){
    g_editor_data_manager->export_animators(m_animators, g_fini->get_value<std::string>("last", "folder"));
    g_s_pressed = false;
    g_ctrl_pressed = false;
  }

  if(m_selected_animator != nullptr){
    for(auto it = m_selected_animator->animations.cbegin(); it != m_selected_animator->animations.cend();){
      if(it->second.should_delete){
        it = m_selected_animator->animations.erase(it);
      }else{
        ++it;
      }
    }
  }


  float dt = Timer::get_dt();

  if(m_selected_animator != nullptr){
    for(auto& [name, animation] : m_selected_animator->animations){
      if(animation.is_playing){
        if(animation.current_timer > m_frame_time){
          animation.current_frame++;
          animation.current_timer = 0;
        }
        animation.current_timer += dt;

        if(animation.current_frame > animation.frames){
          animation.current_frame = 0;
        }
      }
    }
  }
}

void AnimatorView::dispose() {}

void AnimatorView::draw() {}
