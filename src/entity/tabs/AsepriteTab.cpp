#include "AsepriteTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"
#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../game/Game.hpp"
#include "../../res/Res.hpp"
#include "../../renderer/Camera.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "../editors/modules/AsepriteViewer.hpp"
#include "TabUtils.hpp"
#include <memory>
#include "SDL_gpu.h"
#include "SDL.h"

AsepriteTab::AsepriteTab(const std::string& _name) {
  name = _name;
  m_viewer = new RendererViewer();
}

void AsepriteTab::open() {
  reload();
}

void AsepriteTab::show() {
  TabUtils::tab(name);
}

void AsepriteTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }

  m_viewer->update();
  g_camera->set_zoom(3);
}

void AsepriteTab::dispose() {
  g_engine->get_game()->m_viewers.erase(name);
}

void AsepriteTab::draw() {
  //renderer + imgui windows
  if (m_asset != nullptr) {
    TabUtils::asset_header(m_asset);
  }
  ImGui::SameLine();
  ImGui::BeginChild("Aseprite Tab", ImVec2(640, 760), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  auto size = ImGui::GetWindowSize();
  auto pos = ImGui::GetWindowPos();


  GPU_SetActiveTarget(g_engine->get_gpu());
  GPU_SetCamera(g_engine->get_gpu(), nullptr);
  m_viewer->draw({size.x - 16, size.y - 16}, {pos.x + 8, pos.y + 8});
  GPU_SetActiveTarget(nullptr);
  GPU_DeactivateShaderProgram();
  ImGui::EndChild();



}

void AsepriteTab::reload() {
  //load the aseprite file
  if (m_asset != nullptr) {
    m_asset->start();
    m_file_path = m_asset->data["aseprite_path"].value;

    m_ase = g_res->load_aseprite(m_file_path.c_str());
    if(m_ase){
      Logger::log("Loaded aseprite: " + m_file_path);
    }

    m_aseprite_viewer = std::make_shared<AsepriteViewer>(m_ase);
    g_engine->get_game()->m_viewers[name] = m_aseprite_viewer;
    m_viewer->add_module(m_aseprite_viewer);
  }
}

void AsepriteTab::save() {
  is_dirty = false;
  m_asset->start();
  //g_editor_data_manager->m_current_path = m_current_path_buffer;
  //g_folder_path = m_folder_path_buffer;
}

void AsepriteTab::add_asset(Asset* asset) {
  Logger::log("Setting asset: " + asset->file_name);
  m_asset = asset;
}
