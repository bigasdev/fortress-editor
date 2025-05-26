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
  reload();
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
  //load all the assets from the project 
  std::vector<std::string> asset_files;
  auto query = FUtils::get_all_files_in_folder(g_asset_manager->get_asset("Editor Profile")->data["folder_path"].value + "\\res\\assets\\palletes", asset_files);
  for(auto const& file : asset_files) {
    if(FUtils::file_exists(file)) {
      Logger::log("Loading asset files: " + file);
    }
  }
  //load all the components from the project
}

void PrefabTab::save() {
  is_dirty = false;
  m_asset->start();
}

void PrefabTab::add_asset(Asset* asset) {
  Logger::log("Setting asset: " + asset->file_name);
  m_asset = asset;
}
