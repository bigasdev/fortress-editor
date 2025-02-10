#include "Game.hpp"
#include "../core/Engine.hpp"
#include "../core/InputManager.hpp"
#include "../core/EditorDataManager.hpp"
#include "../core/Timer.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../renderer/Camera.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/Sprite.hpp"
#include "../res/Res.hpp"
#include "../tools/Cooldown.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "../tools/Mouse.hpp"
#include "DataLoader.hpp"
#include "Fini.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include "SDL_keycode.h"
#include "json.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>

// Systems
#include "../core/UndoManager.hpp"

// Components
#include "../entity/visualizers/AssetScreen.hpp"
#include "../entity/editors/AssetView.hpp"
#include "../entity/editors/AnimatorView.hpp"
#include "../entity/editors/MainMenu.hpp"
#include "../entity/editors/SideMenu.hpp"

struct Asset {
  char asset_name[128] = "asset";
  std::string file_name;
  Sprite spr;
  std::vector<Animation> animations;
};

std::unique_ptr<EditorDataManager> m_editor_data_manager;

std::string project_folder;
std::string asset_folder;
vec2 pos = {0, 0};
std::map<std::string, Sprite> sprite_map;
std::vector<std::unique_ptr<Asset>> m_assets;
std::unique_ptr<Asset> *m_selected_asset;
std::string selected_asset;
std::string current_asset = "asset";

vec2 mouse_pos;
bool mouse_clicked;
bool mouse_wheel_clicked;
bool mouse_not_clicked;

bool ctrl_pressed = false;
bool load_project = false;
bool save_pressed = false;
bool z_pressed = false;
bool load_assets = false;

// Systems
SpriteAnimator *m_sprite_animator;
Fini *fini;
UndoManager *m_undo_manager;

// Components
std::unique_ptr<SideMenu> side_menu;
std::unique_ptr<MainMenu> main_menu;
std::unique_ptr<AssetView> asset_view;
std::unique_ptr<AnimatorView> animator_view;
std::unique_ptr<AssetScreen> asset_screen;

Game::Game() {}

Game::~Game() {
  fini->save();
}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();
  m_undo_manager = new UndoManager();
  m_editor_data_manager = std::make_unique<EditorDataManager>();
  g_editor_data_manager = m_editor_data_manager.get();
  g_undo_manager = m_undo_manager;

  //

  // initial settings to get last folder and asset
  fini = new Fini("res/config.ini");
  fini->initialize_value("last", "folder", "");
  fini->initialize_value("last", "asset", "");
  g_fini = fini;

  project_folder = fini->get_value<std::string>("last", "folder");

  // this will crash if the saved folder or assets are not found anymore
  // FIX:
  if (project_folder != "") {
    g_res->reset_aseprites();
    g_res->load_aseprites(project_folder + "/res/");
    g_res->load_prefabs(project_folder + "/res/prefabs/");
    sprite_map.clear();
  }

  asset_folder = fini->get_value<std::string>("last", "asset");
  if (asset_folder != "") {
  }

  g_cooldown = m_cooldown;
  g_camera = m_camera;

  auto files = g_res->get_aseprite_names();
  for (auto file : files) {
    Logger::log(file);

    auto spr = Sprite();

    sprite_map[file] = spr;
  }

  // basic camera tracking to make everything in the middle of the screen
  g_camera->track_pos(&pos);

  g_input_manager->bind_mouse(&mouse_clicked, nullptr, &mouse_wheel_clicked);
  g_input_manager->bind_keyboard(SDLK_s, &g_s_pressed);
  g_input_manager->bind_keyboard(SDLK_p, &load_assets);
  g_input_manager->bind_keyboard(SDLK_e, &load_project);
  g_input_manager->bind_keyboard(SDLK_z, &z_pressed);
  g_input_manager->bind_keyboard(SDLK_o, &g_o_pressed);
  g_input_manager->bind_keyboard(SDLK_LCTRL, &g_ctrl_pressed);
  g_input_manager->bind_keyboard(SDLK_RETURN, &g_enter_pressed);
  g_input_manager->bind_keyboard(SDLK_DELETE, &g_del_pressed);


  side_menu = std::make_unique<SideMenu>();
  main_menu = std::make_unique<MainMenu>();
  asset_view = std::make_unique<AssetView>(sprite_map, project_folder);
  animator_view = std::make_unique<AnimatorView>();
  asset_screen = std::make_unique<AssetScreen>();
}

void Game::fixed_update(double tmod) {}

void Game::update(double dt) {
  m_cooldown->update(dt);

  mouse_not_clicked = !mouse_clicked;


  // LOAD PROJECT FOLDER
  if (g_ctrl_pressed and load_project) {
    project_folder = Data_Loader::load_folder("Select project folder");
    if (project_folder == "")
      return;
    fini->set_value("last", "folder", project_folder);
    g_res->reset_aseprites();
    g_res->load_aseprites(project_folder + "/res/");
    sprite_map.clear();

    auto files = g_res->get_aseprite_names();
    for (auto file : files) {
      Logger::log(file);

      auto spr = Sprite();

      sprite_map[file] = spr;
    }

    load_project = false;
    ctrl_pressed = false;
    asset_view->update_sprite_map(project_folder, sprite_map);
  }

  // UNDO
  if (g_ctrl_pressed and z_pressed) {
    m_undo_manager->undo();
    z_pressed = false;
    ctrl_pressed = false;
  }

  // components updates
  if (side_menu->get_state() == State::ASSET) {
    asset_screen->update();
    asset_view->update();
  }

  if (side_menu->get_state() == State::ANIMATOR) {
    animator_view->update();
  }
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {
  if (side_menu->get_state() == State::ASSET) {
    asset_screen->root();
  }
}

void Game::draw_ent() {
  if (side_menu->get_state() == State::ASSET) {
    asset_screen->ent();
  }
}

void Game::draw_ui() {
  if (side_menu->get_state() == State::ASSET) {
    asset_screen->ui();
    asset_view->draw();
  }

  if (side_menu->get_state() == State::ANIMATOR) {
    animator_view->draw();
  }
}

void Game::imgui_assets() {}

int x = 16;
int y = 16;
void Game::imgui_map() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::SetNextWindowSize(
      ImVec2(g_engine->get_window_size()->x, g_engine->get_window_size()->y));
  ImGui::Begin("Workspace", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoMouseInputs |
                   ImGuiWindowFlags_NoScrollbar);

  side_menu->show();
  //
  main_menu->show();

  if (side_menu->get_state() == State::ASSET) {
    asset_view->show();
  }

  if (side_menu->get_state() == State::ANIMATOR) {
    animator_view->show();
  }

  if (side_menu->get_state() == State::NONE) {
  }
  ImGui::PopStyleColor();
  ImGui::End();
}

void Game::draw_imgui() {
  GUI::draw([this]() { this->imgui_map(); });
}

void Game::save() {
  nlohmann::json j;
  for (auto &asset : m_assets) {
    nlohmann::json asset_j;
    asset_j["asset_name"] = asset->asset_name;
    asset_j["file_name"] = asset->file_name;
    asset_j["dst_x"] = asset->spr.dst_x;
    asset_j["dst_y"] = asset->spr.dst_y;
    asset_j["wid"] = asset->spr.wid;
    asset_j["hei"] = asset->spr.hei;

    j.push_back(asset_j);
  }
  std::ofstream o;
  if (asset_folder == "") {
    std::ofstream o("res/map.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  } else {
    std::ofstream o(asset_folder);
    o << std::setw(4) << j << std::endl;
    o.close();
  }
}

void Game::load(std::string file_path) {
  std::ifstream i(file_path);
  nlohmann::json j;
  i >> j;
  m_selected_asset = nullptr;
  m_assets.clear();

  for (auto &asset_j : j) {
    Asset asset;
    std::strcpy(asset.asset_name,
                asset_j["asset_name"].get<std::string>().c_str());
    asset.file_name = asset_j["file_name"];
    asset.spr.dst_x = asset_j["dst_x"];
    asset.spr.dst_y = asset_j["dst_y"];
    asset.spr.wid = asset_j["wid"];
    asset.spr.hei = asset_j["hei"];

    m_assets.push_back(std::make_unique<Asset>(asset));
  }
}

void Game::clean() { delete fini; }
