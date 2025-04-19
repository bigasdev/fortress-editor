#include "EditorDataManager.hpp"
#include "DataLoader.hpp"
#include "Fini.hpp"
#include "json.hpp"
#include "../entity/data/EntityData.hpp"
#include "../entity/editors/AnimatorView.hpp"
#include "../tools/Logger.hpp"
#include <fstream>

#include "UndoManager.hpp"
#include "global.hpp"
#include "../res/Res.hpp"
#include "../entity/actions/AssetAction.hpp"
#include "../renderer/Sprite.hpp"

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
    entity.group = asset["group"];
    entity.pallete_name = asset["atlas_name"];
    entity.atlas_pos.x = asset["atlas_pos_x"];
    entity.atlas_pos.y = asset["atlas_pos_y"];
    entity.sprite_size.x = asset["sprite_size_x"];
    entity.sprite_size.y = asset["sprite_size_y"];
    entity.collision_box.x = asset["collision_box_x"];
    entity.collision_box.y = asset["collision_box_y"];
    entity.collision_offset.x = asset["collision_offset_x"];
    entity.collision_offset.y = asset["collision_offset_y"];
    entity.sprite_offset.x = asset["sprite_offset_x"];
    entity.sprite_offset.y = asset["sprite_offset_y"];

    auto components = Data_Loader::get_files(g_folder_path + "/src/components/", ".hpp");
    for(auto &component : components) {
      ComponentData data;
      auto name = component.substr(0, component.find_last_of('.'));
      data.name = name;
      if(data.name == "ComponentStore" || data.name == "IComponent") continue;
      if(data.name == "SpriteComponent" || data.name == "TransformComponent"){
        data.is_active = true;
      }
      entity.components[name] = data;
    }

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
    asset_j["group"] = asset.second.group;
    asset_j["atlas_name"] = asset.second.pallete_name;
    asset_j["atlas_pos_x"] = asset.second.atlas_pos.x;
    asset_j["atlas_pos_y"] = asset.second.atlas_pos.y;
    asset_j["sprite_size_x"] = asset.second.sprite_size.x;
    asset_j["sprite_size_y"] = asset.second.sprite_size.y;
    asset_j["collision_box_x"] = asset.second.collision_box.x;
    asset_j["collision_box_y"] = asset.second.collision_box.y;
    asset_j["collision_offset_x"] = asset.second.collision_offset.x;
    asset_j["collision_offset_y"] = asset.second.collision_offset.y;
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

void EditorDataManager::export_animators(std::map<std::string, Animator> animators, std::string path){
  if(path == ""){
    path = Data_Loader::load_folder("Select a folder to save the animators");
  }
  Logger::log("Exporting animators to: " + path + "/res/animations/");


  nlohmann::json j;
  for (auto &animator : animators) {
    nlohmann::json animator_j;
    animator_j["name"] = animator.first;
    animator_j["sprite_name"] = animator.second.sprite->sheet;

    nlohmann::json animations_j;
    for(auto &animation : animator.second.animations){
      nlohmann::json animation_j;
      animation_j["name"] = animation.first;
      animation_j["x"] = animation.second.x;
      animation_j["y"] = animation.second.y;
      animation_j["frames"] = animation.second.frames;
      animation_j["loop"] = animation.second.loop;
      animation_j["block_transition"] = animation.second.block_transition;

      animations_j.push_back(animation_j);
    }

    animator_j["animations"] = animations_j;
    j.push_back(animator_j);
  }
  std::ofstream o;
  
  if(path == m_current_path){
    std::ofstream o(path);
    o << std::setw(4) << j << std::endl;
    o.close();
  }

  if (path == "") {
    std::ofstream o("res/animators.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  } else {
    std::ofstream o(path + "/res/animations/animations.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  }

  g_fini->set_value("last", "animator", path + "/res/animations/animations.json");
}

std::map<std::string, Animator> EditorDataManager::import_animators(std::string path){
  if(path == ""){
    path = Data_Loader::load_file("json");
  }
  Logger::log("Importing animators from: " + path);
  m_current_path = path;

  std::ifstream i(path);
  nlohmann::json j;
  i >> j;

  std::map<std::string, Animator> animators;
  for (auto &animator : j) {
    Logger::log("Importing animator: ");
    Animator anim;
    anim.name = animator["name"];
    anim.sprite = g_res->get_sprite(animator["sprite_name"]);

    for(auto &animation : animator["animations"]){
      Animation anima;
      anima.name = animation["name"];
      anima.x = animation["x"];
      anima.y = animation["y"];
      anima.frames = animation["frames"];
      anima.loop = animation["loop"];
      anima.block_transition = animation["block_transition"];

      anim.animations[anima.name] = anima;
    }

    animators[anim.name] = anim;
  }

  i.close();
  return animators;
}




