#include "PrefabTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"

#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "TabUtils.hpp"

PrefabTab::PrefabTab(const std::string& _name) {
  name = _name;

  //m_viewer = new RendererViewer();
}

void PrefabTab::open() {
}

void PrefabTab::show() {
  TabUtils::tab(name);
}

void PrefabTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }

  //m_viewer->update();
}

void PrefabTab::dispose() {
  //delete m_viewer;
}

void PrefabTab::draw() {
  if (m_asset != nullptr) {
    TabUtils::asset_header(m_asset);
    return;
  }

}

void PrefabTab::reload() {
}

void PrefabTab::save() {
  is_dirty = false;
  m_asset->start();
}

void PrefabTab::add_asset(Asset* asset) {
  Logger::log("Setting asset: " + asset->file_name);
  m_asset = asset;
}
