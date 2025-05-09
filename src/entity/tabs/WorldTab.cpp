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
  TabUtils::tab("World");
}

void WorldTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }
}

void WorldTab::dispose() {
}

void WorldTab::draw() {
  TabUtils::asset_header(m_asset);
}

void WorldTab::reload() {
}

void WorldTab::save() {
  is_dirty = false;
  m_asset->start();
}
