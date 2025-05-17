#include "AsepriteTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"
#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "TabUtils.hpp"

AsepriteTab::AsepriteTab(const std::string& _name) {
  name = _name;
  m_viewer = new RendererViewer();
}

void AsepriteTab::open() {
}

void AsepriteTab::show() {
  TabUtils::tab(name);
}

void AsepriteTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }

  //m_viewer->update();
}

void AsepriteTab::dispose() {
  delete m_viewer;
}

void AsepriteTab::draw() {
  //renderer + imgui windows
  if(m_viewer->is_mouse_on_area()){
    auto coord = m_viewer->get_coordinate();
    ImGui::Text("X: %.2f Y: %.2f", coord.x, coord.y);
    ImGui::NewLine();
  }

  ImGui::BeginChild("Aseprite Tab", ImVec2(408, 408), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  auto size = ImGui::GetWindowSize();
  auto pos = ImGui::GetWindowPos();


  //m_viewer->draw({size.x - 16, size.y - 16}, {pos.x + 8, pos.y + 8});
  ImGui::EndChild();


  if (m_asset != nullptr) {
    TabUtils::asset_header(m_asset);
    return;
  }

}

void AsepriteTab::reload() {
  //load the aseprite file
  if (m_file_path != "") {

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
