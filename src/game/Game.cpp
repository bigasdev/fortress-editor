#include "Game.hpp"
#include "../core/Engine.hpp"
#include "../core/InputManager.hpp"
#include "../core/Timer.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../renderer/Camera.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/Sprite.hpp"
#include "../res/Res.hpp"
#include "../tools/Cooldown.hpp"
#include "../tools/FUtils.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "../tools/Mouse.hpp"
#include "DataLoader.hpp"
#include "Editor.hpp"
#include "Fini.hpp"
#include "Pallete.hpp"
#include "Prefab.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include "SDL_keycode.h"
#include "json.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>

// Systems
Fini *fini;

// Views
std::unique_ptr<Editor> m_editor;
std::unique_ptr<Pallete> m_pallete;
std::unique_ptr<Prefab> m_prefab;

Game::Game() {}

Game::~Game() { fini->save(); }

void Game::init() {
  // g_editor_folder_path = FUtils::get_current_path();

  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();

  // initial settings to get last folder and asset
  /*fini = new Fini("res/config.ini");
  fini->initialize_value("last", "folder", "");
  g_folder_path = fini->get_value<std::string>("last", "folder");
  fini->initialize_value("last", "asset", "");
  fini->initialize_value("settings", "grid_size", 16);
  fini->initialize_value("settings", "zoom", 1.0f);
  fini->initialize_value("settings", "zoom", 1.0f);
  g_fini = fini;*/

  fini = new Fini("res/editor.ini");
  g_fini = fini;

  Logger::log("Game init");

  g_cooldown = m_cooldown;
  g_camera = m_camera;

  auto files = g_res->get_aseprite_names();
  for (auto file : files) {
    Logger::log(file);

    auto spr = Sprite();
  }

  // FIX: rework the input manager
  g_input_manager->bind_mouse(&g_left_click, &g_right_click, &g_left_click);

  m_editor = std::make_unique<Editor>();
  m_editor->init();
  m_pallete = std::make_unique<Pallete>();
  m_pallete->init();
  m_prefab = std::make_unique<Prefab>();
  m_prefab->init();
}

void Game::fixed_update(double tmod) {}

void Game::update(double dt) {
  if (g_input_manager->get_key_press(SDLK_e, SDLK_LCTRL)) {
    m_current_tab = Tab::EDITOR;
  }
  if (g_input_manager->get_key_press(SDLK_p, SDLK_LCTRL)) {
    m_current_tab = Tab::PALLETES;
  }
  if (g_input_manager->get_key_press(SDLK_f, SDLK_LCTRL)) {
    m_current_tab = Tab::PREFABS;
  }

  switch (m_current_tab) {
  case Tab::EDITOR:
    m_editor->update();
    break;
  case Tab::PALLETES:
    m_pallete->update();
    break;
  case Tab::PREFABS:
    m_prefab->update();
    break;
  default:
    Logger::log("Unknown tab selected");
    break;
  }

  m_cooldown->update(dt);
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {
  /*if (side_menu->get_state() == State::ASSET) {
    asset_screen->root();
  }*/
}

void Game::draw_ent() {
  /*if (side_menu->get_state() == State::ASSET) {
    asset_screen->ent();
  }*/
  if (m_current_tab == Tab::PALLETES) {
    m_pallete->draw();
  } else if (m_current_tab == Tab::PREFABS) {
    m_prefab->draw();
  }
}

void Game::draw_ui() {}

void Game::draw_viewers() {}

void Game::imgui_assets() {}

int x = 16;
int y = 16;
void Game::imgui_map() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(
      ImVec2(g_engine->get_window_size()->x, g_engine->get_window_size()->y));
  ImGui::Begin("Workspace", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoMouseInputs |
                   ImGuiWindowFlags_NoScrollbar);

  // tab selection, i.e the main screen at the editor
  ImGui::BeginChild("Tabs", ImVec2(150, 0), true,
                    ImGuiWindowFlags_AlwaysUseWindowPadding);
  if (ImGui::Button(" Editor")) {
    m_current_tab = Tab::EDITOR;
    m_editor->init();
  }
  if (ImGui::Button(" Palettes")) {
    m_current_tab = Tab::PALLETES;
    m_pallete->init();
  }
  if (ImGui::Button(" Prefabs")) {
    m_current_tab = Tab::PREFABS;
    m_prefab->init();
  }
  ImGui::EndChild();
  ImGui::SameLine();
  if (m_current_tab != Tab::EDITOR) {
    ImGui::BeginChild("SideMenu", ImVec2(150, 0), true,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    switch (m_current_tab) {
    case Tab::PALLETES:
      m_pallete->side_draw();
      break;
    case Tab::PREFABS:
      m_prefab->side_draw();
      break;
    default:
      ImGui::Text("Unknown tab selected");
      break;
    }

    ImGui::EndChild();
  }
  ImGui::SameLine();
  ImGui::BeginChild("MainContent", ImVec2(0, 0), true,
                    ImGuiWindowFlags_AlwaysUseWindowPadding);
  switch (m_current_tab) {
  case Tab::EDITOR:
    m_editor->draw();
    break;
  case Tab::PALLETES:
    break;
  case Tab::PREFABS:
    break;
  default:
    ImGui::Text("Unknown tab selected");
    break;
  }
  ImGui::EndChild();

  ImGui::End();
}

void Game::draw_imgui() {
  GUI::draw([this]() { this->imgui_map(); });
}

void Game::save() {
  /*nlohmann::json j;
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
  }*/
}

void Game::load(std::string file_path) {
  /*std::ifstream i(file_path);
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
  }*/
}

void Game::clean() { g_fini->save(); }
