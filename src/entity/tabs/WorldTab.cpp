#include "WorldTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"

#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "TabUtils.hpp"

WorldTab::WorldTab(const std::string& _name) {
  name = _name;

  m_viewer = new RendererViewer();
}

void WorldTab::open() {
}

void WorldTab::show() {
  TabUtils::tab(name);
}

void WorldTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }

  m_viewer->update();
}

void WorldTab::dispose() {
  delete m_viewer;
}

void WorldTab::draw() {
  //renderer + imgui windows
  if(m_viewer->is_mouse_on_area()){
    auto coord = m_viewer->get_coordinate();
    ImGui::Text("X: %.2f Y: %.2f", coord.x, coord.y);
    ImGui::NewLine();
  }

  ImGui::BeginChild("World Tab", ImVec2(408, 408), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  auto size = ImGui::GetWindowSize();
  auto pos = ImGui::GetWindowPos();


  m_viewer->draw({size.x - 16, size.y - 16}, {pos.x + 8, pos.y + 8});
  ImGui::EndChild();


  if (m_asset != nullptr) {
    TabUtils::asset_header(m_asset);
    return;
  }

}

void WorldTab::reload() {
}

void WorldTab::save() {
  is_dirty = false;
  m_asset->start();
}

void WorldTab::add_asset(Asset* asset) {
  Logger::log("Setting asset: " + asset->file_name);
  m_asset = asset;
}
