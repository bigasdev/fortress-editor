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

    for(auto const& component : m_components_list) {
      if(ImGui::Button("")){
        //FIX: 
        //
      }
      ImGui::SameLine();
      if(ImGui::CollapsingHeader(component.c_str())) {
      }
    }

    if(ImGui::Button("Select component")){
      ImGui::OpenPopup("Select Component");
    }

    if(ImGui::BeginPopup("Select Component")){
      for(auto const& component : m_avaliable_components) {
        if(ImGui::Selectable(component.second.c_str())){
          //check if the component exists before adding it 
          //FIX:
          m_components_list.push_back(component.second);
          Asset component_asset;
          component_asset.data["name"].value = component.second;

          m_asset->children[component.second] = component_asset;

          is_dirty = true;
        }
      }
      ImGui::EndPopup();
    }
  }
  
}

void PrefabTab::reload() {
  //load all the assets from the project 
  Logger::log("Reloading Prefab Tab");
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
  std::vector<std::string> component_files;
  query = FUtils::get_all_files_in_folder(g_asset_manager->get_asset("Editor Profile")->data["folder_path"].value + "\\src\\components", component_files);
  for(auto const& file : component_files) {
    if(FUtils::file_exists(file)) {
      auto file_name = FUtils::get_file_name(file);
      auto file_name_without_ext = file_name.substr(0, file_name.find_last_of('.'));
      //later we will read the component to get the data type

      //sloppy but will do for now 
      //FIX: make a better way to check if its a component or just a cpp file
      if(file_name_without_ext != "IComponent" && file_name_without_ext != "ComponentStore"){
        Logger::log("Loading component files: " + file_name);
        
        m_avaliable_components[file_name_without_ext] = file_name_without_ext;
      }

    }
  }

  if(m_asset != nullptr) {
    //components data 
    m_components_list.clear();
    for(auto& component : m_asset->children) {
      m_components_list.push_back(component.second.data["name"].value);
    }
  }
}

void PrefabTab::save() {
  is_dirty = false;
  

  m_asset->start();
}

void PrefabTab::add_asset(Asset* asset) {
  Logger::log("Setting asset: " + asset->file_name);
  m_asset = asset;
}
