#include "EditorDataManager.hpp"
#include "DataLoader.hpp"
#include "Fini.hpp"
#include "json.hpp"
#include "../entity/data/EntityData.hpp"
#include "../tools/Logger.hpp"
#include <fstream>

#include "UndoManager.hpp"
#include "global.hpp"
#include "../entity/actions/AssetAction.hpp"

EditorDataManager::EditorDataManager(){
}

EditorDataManager::~EditorDataManager(){
}

void EditorDataManager::on_file_drop(std::string path){
}

void EditorDataManager::import(std::map<std::string, EntityData>& assets, std::string path){
  if(path == ""){
    path = Data_Loader::load_file("json");
  }
  Logger::log("Importing assets from: " + path);
  m_current_path = path;
  assets.clear();

  g_fini->set_value("last", "asset", path);

  std::ifstream i(path);
  nlohmann::json j;
  i >> j;

  for (auto &asset : j) {
    Logger::log("Importing asset: ");
    EntityData entity;
    entity.name = asset["name"];
    entity.group = "default";
    entity.pallete_name = asset["atlas_name"];
    entity.atlas_pos.x = asset["atlas_pos_x"];
    entity.atlas_pos.y = asset["atlas_pos_y"];
    entity.sprite_size.x = asset["sprite_size_x"];
    entity.sprite_size.y = asset["sprite_size_y"];
    entity.sprite_offset.x = asset["sprite_offset_x"];
    entity.sprite_offset.y = asset["sprite_offset_y"];

    if(entity.sprite_size.x > 0){
      entity.sprite_pos.x = entity.atlas_pos.x * entity.sprite_size.x;
    }else{
      entity.sprite_pos.x = 0;
    }

    if(entity.sprite_size.y > 0){
      entity.sprite_pos.y = entity.atlas_pos.y * entity.sprite_size.y;
    }else{
      entity.sprite_pos.y = 0;
    }

    assets[entity.name] = entity;
    auto action = new AssetAction(entity, assets);
    g_undo_manager->add(action);

    Logger::log("Assets from the data manager");
    for(auto &asset : assets){
      Logger::log(asset.first);
    }
  }

  i.close();
}

void EditorDataManager::export_(std::map<std::string, EntityData> assets, std::string path){
  if(path == "" && m_current_path == ""){
    path = Data_Loader::load_folder("Select a folder to save the assets");
  }else{
    path = m_current_path;
  }
  Logger::log("Exporting assets to: " + path);

  g_fini->set_value("last", "asset", path);

  nlohmann::json j;
  for (auto &asset : assets) {
    nlohmann::json asset_j;
    
    asset_j["name"] = asset.second.name;
    asset_j["atlas_name"] = asset.second.pallete_name;
    asset_j["atlas_pos_x"] = asset.second.atlas_pos.x;
    asset_j["atlas_pos_y"] = asset.second.atlas_pos.y;
    asset_j["sprite_size_x"] = asset.second.sprite_size.x;
    asset_j["sprite_size_y"] = asset.second.sprite_size.y;
    asset_j["sprite_offset_x"] = asset.second.sprite_offset.x;
    asset_j["sprite_offset_y"] = asset.second.sprite_offset.y;

    j.push_back(asset_j);
  }
  std::ofstream o;
  
  if(path == m_current_path){
    std::ofstream o(path);
    o << std::setw(4) << j << std::endl;
    o.close();
  }

  if (path == "") {
    std::ofstream o("res/map.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  } else {
    std::ofstream o(path + "/map.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  }
}

void EditorDataManager::auto_save(std::map<std::string, EntityData> assets){
  if(m_current_path == ""){
    return;
  }

  export_(assets, m_current_path);
}
