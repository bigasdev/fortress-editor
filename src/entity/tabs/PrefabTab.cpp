#include "PrefabTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"

#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "TabUtils.hpp"
#include <fstream>
#include "json.hpp"

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
  }
  
  ImGui::BeginCombo("##Asset list", m_asset_data.begin()->first.c_str(), ImGuiComboFlags_HeightLarge);
  for(auto& asset : m_asset_data) {
    if(ImGui::Selectable(asset.first.c_str(), false)) {
      m_asset->data["name"].value = asset.first;
      m_asset->data["x"].value = std::to_string(asset.second.x);
      m_asset->data["y"].value = std::to_string(asset.second.y);
      m_asset->data["w"].value = std::to_string(asset.second.w);
      m_asset->data["h"].value = std::to_string(asset.second.h);
    }
  }
  ImGui::EndCombo();
}

void PrefabTab::reload() {
  //load all the assets from the project 
  std::vector<std::string> asset_files;
  auto query = FUtils::get_all_files_in_folder(g_asset_manager->get_asset("Editor Profile")->data["folder_path"].value + "\\res\\assets\\palletes", asset_files);
  for(auto const& file : asset_files) {
    if(FUtils::file_exists(file)) {
      Logger::log("Loading asset files: " + file);

      std::ifstream i(file);
      nlohmann::json j;
      i >> j;

      for(const auto& data : j["data"].items()) {
        std::string value = data.value().get<std::string>();
        auto prefab_data = TabUtils::get_asset_data(value);
        prefab_data.name = data.key();

        if(prefab_data.x == 9898 && prefab_data.y == 9898 && prefab_data.w == 9898 && prefab_data.h == 9898) {
          continue;
        }

        m_asset_data[prefab_data.name] = prefab_data;
      }
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
