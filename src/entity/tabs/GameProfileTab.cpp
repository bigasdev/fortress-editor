#include "GameProfileTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"

#include "../../tools/Logger.hpp"
#include "TabUtils.hpp"

GameProfileTab::GameProfileTab(const std::string& _name) {
  name = _name;

  Asset game_profile_asset;
  game_profile_asset.file_name = "Game Profile";
  game_profile_asset.file_path = g_editor_folder_path + "\\res\\assets\\game_profile.json";
  game_profile_asset.is_static = true;

  IData assets_resource_path;
  IData game_width;
  IData game_height;
  IData game_scale;
  IData game_fullscreen;
  IData game_fps;
  IData fixed_update_factor;
  IData game_title;
  IData game_debug;
  IData starting_world;

  assets_resource_path.name = "assets_resource_path";
  assets_resource_path.value = "res/";
  game_width.name = "game_width";
  game_width.value = "640";
  game_height.name = "game_height";
  game_height.value = "360";
  game_scale.name = "game_scale";
  game_scale.value = "4.0";
  game_fullscreen.name = "game_fullscreen";
  game_fullscreen.value = "false";
  game_fps.name = "game_fps";
  game_fps.value = "60";
  fixed_update_factor.name = "fixed_update_factor";
  fixed_update_factor.value = "0.0166666666666667";
  game_title.name = "game_title";
  game_title.value = "Fortress Engine";
  game_debug.name = "game_debug";
  game_debug.value = "true";
  starting_world.name = "starting_world";
  starting_world.value = "title_screen";

  game_profile_asset.data["assets_resource_path"] = assets_resource_path;
  game_profile_asset.data["game_width"] = game_width;
  game_profile_asset.data["game_height"] = game_height;
  game_profile_asset.data["game_scale"] = game_scale;
  game_profile_asset.data["game_fullscreen"] = game_fullscreen;
  game_profile_asset.data["game_fps"] = game_fps;
  game_profile_asset.data["fixed_update_factor"] = fixed_update_factor;
  game_profile_asset.data["game_title"] = game_title;
  game_profile_asset.data["game_debug"] = game_debug;
  game_profile_asset.data["starting_world"] = starting_world;

  g_asset_manager->add_asset("Game Profile", game_profile_asset);

  m_asset = g_asset_manager->get_asset("Game Profile");
}

void GameProfileTab::open() {
}

void GameProfileTab::show() {
  TabUtils::tab("Game Profile");
}

void GameProfileTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }
}

void GameProfileTab::dispose() {
}

void GameProfileTab::draw() {
  TabUtils::asset_header(m_asset);
}

void GameProfileTab::reload() {
}

void GameProfileTab::save() {
  is_dirty = false;
  m_asset->start();
}
