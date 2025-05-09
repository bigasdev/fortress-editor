#include "GameProfileTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"

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

  m_game_width = std::stoi(m_asset->data["game_width"].value);
  m_game_height = std::stoi(m_asset->data["game_height"].value);
  m_game_scale = std::stof(m_asset->data["game_scale"].value);
  m_game_fullscreen = m_asset->data["game_fullscreen"].value == "true" ? true : false;
  m_game_fps = std::stoi(m_asset->data["game_fps"].value);
  m_fixed_update_factor = std::stof(m_asset->data["fixed_update_factor"].value);
  m_game_debug = m_asset->data["game_debug"].value == "true" ? true : false;

  auto editor_profile = g_asset_manager->get_asset("Editor Profile");
  m_asset->file_path = editor_profile->data["folder_path"].value + "game_profile.json";
}

void GameProfileTab::open() {
}

void GameProfileTab::show() {
  TabUtils::tab("Game Profile");
}

void GameProfileTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;

    m_asset->data["game_width"].value = std::to_string(m_game_width);
    m_asset->data["game_height"].value = std::to_string(m_game_height);
    m_asset->data["game_scale"].value = std::to_string(m_game_scale);
    m_asset->data["game_fullscreen"].value = m_game_fullscreen ? "true" : "false";
    m_asset->data["game_fps"].value = std::to_string(m_game_fps);
    m_asset->data["fixed_update_factor"].value = std::to_string(m_fixed_update_factor);
    m_asset->data["game_debug"].value = m_game_debug ? "true" : "false";
  }
}

void GameProfileTab::dispose() {
}

void GameProfileTab::draw() {
  TabUtils::asset_header(m_asset);
  ImGuiUtils::header_input_text("Assets Resource Path", &m_asset->data["assets_resource_path"].value);
  ImGuiUtils::header_input_int("Game Width", &m_game_width);
  ImGuiUtils::header_input_int("Game Height", &m_game_height);
  ImGuiUtils::header_input_float("Game Scale", &m_game_scale);
  ImGuiUtils::header_input_bool("Game Fullscreen", &m_game_fullscreen);
  ImGuiUtils::header_input_int("Game FPS", &m_game_fps);
  ImGuiUtils::header_input_float("Fixed Update Factor", &m_fixed_update_factor);
  ImGuiUtils::header_input_text("Game Title", &m_asset->data["game_title"].value);
  ImGuiUtils::header_input_bool("Game Debug", &m_game_debug);
  ImGuiUtils::header_input_text("Starting World", &m_asset->data["starting_world"].value);
}

void GameProfileTab::reload() {
}

void GameProfileTab::save() {
  is_dirty = false;
  m_asset->start();
}
