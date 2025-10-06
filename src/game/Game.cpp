#include "Game.hpp"
#include "../core/DebugLayer.hpp"
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
  m_camera->set_game_scale(2);
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
  fini->initialize_value("memory", "tab", "editor");
  auto tab = fini->get_value<std::string>("memory", "tab");
  if (tab == "editor") {
    m_current_tab = Tab::EDITOR;
  } else if (tab == "pallete") {
    m_current_tab = Tab::PALLETES;
  } else if (tab == "prefab") {
    m_current_tab = Tab::PREFABS;
  } else {
    Logger::log("Unknown tab in memory, defaulting to editor");
    m_current_tab = Tab::EDITOR;
  }
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
  if (g_input_manager->get_key_press(SDLK_m)) {
    m_debug_layer_open = !m_debug_layer_open;
  }
  /*if (g_input_manager->get_key_press(SDLK_f, SDLK_LCTRL)) {
    m_current_tab = Tab::PREFABS;
  }*/

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
                   ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoBringToFrontOnFocus);

  // tab selection, i.e the main screen at the editor
  ImGui::BeginChild("Tabs", ImVec2(150, 0), true,
                    ImGuiWindowFlags_AlwaysUseWindowPadding);

  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImVec2 start_pos = ImGui::GetCursorScreenPos();

  float button_width = 122.0f;
  float button_height = 30.0f;
  float spacing = 10.0f;

  auto tab_button = [&](const char *label, Tab tab, ImU32 color) {
    ImVec2 p = ImGui::GetCursorScreenPos();

    draw_list->AddRectFilled(p, ImVec2(p.x + button_width, p.y + button_height),
                             color);

    ImGui::SetCursorScreenPos(ImVec2(p.x + spacing, p.y));
    if (ImGui::Button(label, ImVec2(button_width, button_height))) {
      m_current_tab = tab;
      switch (tab) {
      case Tab::EDITOR:
        g_fini->set_value("memory", "tab", "editor");
        m_editor->init();
        break;
      case Tab::PALLETES:
        g_fini->set_value("memory", "tab", "pallete");
        m_pallete->init();
        break;
      case Tab::PREFABS:
        g_fini->set_value("memory", "tab", "prefab");
        m_prefab->init();
        break;
      default:
        Logger::log("Unknown tab selected");
        break;
      }
    }
  };

  tab_button("  Editor", Tab::EDITOR, IM_COL32(63, 113, 166, 255));
  tab_button(" Palettes", Tab::PALLETES, IM_COL32(210, 88, 38, 255));
  tab_button(" Data", Tab::PREFABS, IM_COL32(90, 139, 90, 255));

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
                    ImGuiWindowFlags_AlwaysUseWindowPadding |
                        ImGuiWindowFlags_NoBringToFrontOnFocus);
  switch (m_current_tab) {
  case Tab::EDITOR:
    m_editor->draw();
    break;
  case Tab::PALLETES:
    break;
  case Tab::PREFABS:
    m_prefab->draw();
    break;
  default:
    ImGui::Text("Unknown tab selected");
    break;
  }
  ImGui::EndChild();

  ImGui::End();

#if _DEBUG
  if (m_debug_layer_open) {
    DebugLayer::draw_debug_layer();
  }
#endif
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
