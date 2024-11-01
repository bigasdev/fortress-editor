#include "EditorDataManager.hpp"
#include "DataLoader.hpp"
#include "Fini.hpp"
#include "json.hpp"
#include "../entity/data/EntityData.hpp"

EditorDataManager::EditorDataManager(){
}

EditorDataManager::~EditorDataManager(){
}

void EditorDataManager::on_file_drop(std::string path){
}

void EditorDataManager::import(std::string path){
}

void EditorDataManager::export_(std::map<std::string, EntityData> assets, std::string path){
  if(path == ""){
    path = Data_Loader::load_folder("Select a folder to save the assets");
  }

  nlohmann::json j;
  for (auto &asset : assets) {
    nlohmann::json asset_j;
    
    asset_j["name"] = asset.second.name;
    asset_j["atlas_name"] = asset.second.pallete_name;
    asset_j["atlas_pos_x"] = asset.second.atlas_pos.x;
    asset_j["atlas_pos_y"] = asset.second.atlas_pos.y;
    asset_j["sprite_size_x"] = asset.second.sprite_size.x;
    asset_j["sprite_size_y"] = asset.second.sprite_size.y;

    j.push_back(asset_j);
  }
  std::ofstream o;
  if (path == "") {
    std::ofstream o("res/map.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  } else {
    std::ofstream o(path);
    o << std::setw(4) << j << std::endl;
    o.close();
  }
}
