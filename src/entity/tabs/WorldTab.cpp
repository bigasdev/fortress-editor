#include "WorldTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"

#include "../../tools/Logger.hpp"
#include "TabUtils.hpp"

WorldTab::WorldTab(const std::string& _name) {
  name = _name;
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
}

void WorldTab::dispose() {
}

void WorldTab::draw() {
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
