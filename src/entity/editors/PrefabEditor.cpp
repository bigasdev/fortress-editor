#include "PrefabEditor.hpp"

#include "../../entity/tabs/TabUtils.hpp"
#include "../../entity/tabs/WorldTab.hpp"
#include "../../entity/tabs/TabsGenerator.hpp"
#include "../../tools/FUtils.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include "json.hpp"

ImVec2 p_mouse_pos = ImVec2(0, 0);

PrefabEditor::PrefabEditor() {
    //creating the static needed assets
  Asset editor_profile_asset;
  editor_profile_asset.file_name = "Editor Profile";
  editor_profile_asset.file_path = g_editor_folder_path + "\\res\\assets\\editor_profile.json";
  editor_profile_asset.is_static = true;

  IData folder_path;
  IData current_path;
  folder_path.name = "folder_path";
  folder_path.value = g_editor_folder_path;
  current_path.name = "current_path";
  current_path.value = g_editor_folder_path + "/res/editor_profile.json";

  editor_profile_asset.data["folder_path"] = folder_path;
  editor_profile_asset.data["current_path"] = current_path;

  g_asset_manager->add_asset("Editor Profile", editor_profile_asset);
}

void PrefabEditor::open() {
    reload();
}

void PrefabEditor::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
  if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()){
    m_item_clicked = NONE;
  }

  if(m_item_clicked != NONE){
    create_asset_popup(m_item_clicked);
  }

  if(ImGui::CollapsingHeader(" Prefabs")){
    handle_click_asset(PREFAB, m_prefabs);
  }

  if(ImGui::CollapsingHeader(" Database")){
    handle_click_asset(DATABASE, m_databases);
  }

  if(ImGui::CollapsingHeader(" World")){
    handle_click_asset(WORLD, m_worlds);
  }
    
  if(ImGui::Button(" Editor Profile")){
    auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab("Editor Profile");
    if(tab == nullptr){
      Logger::log("Editor Profile Tab not found");
      return;
    }

    g_editor_manager->get_editor<TabsWindowEditor>()->switch_open_tab("Editor Profile");
    g_editor_manager->get_editor<TabsWindowEditor>()->unselect_tab("Editor Profile");
  }
  if(ImGui::Button(" Game Profile")){
    auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab("Game Profile");
    if(tab == nullptr){
      Logger::log("Game Profile Tab not found");
      return;
    }

    g_editor_manager->get_editor<TabsWindowEditor>()->switch_open_tab("Game Profile");
    g_editor_manager->get_editor<TabsWindowEditor>()->unselect_tab("Game Profile");
  }

  ImGui::PopStyleColor();
  ImGui::End();
}

void PrefabEditor::update(){

}

void PrefabEditor::dispose() {
  Logger::log("Prefab Editor disposed");
}

void PrefabEditor::draw() {
}

void PrefabEditor::reload() {
  Logger::log("Reloading Prefab Editor");
  //load all asset files
  m_worlds.clear();
  Asset* game_profile_asset = g_asset_manager->get_asset("Editor Profile");
  if(game_profile_asset == nullptr){
    Logger::log("Game Profile Asset not found");
    return;
  }

  //Prefabs
  //db 
  //world
  if(FUtils::folder_exists(game_profile_asset->data["folder_path"].value + "\\res\\assets\\worlds")){
    std::vector<std::string> worlds;
    auto query = FUtils::get_all_files_in_folder(game_profile_asset->data["folder_path"].value + "\\res\\assets\\worlds", worlds);
    for (const auto& world : worlds) {
      if (FUtils::file_exists(world)) {
        std::ifstream i(world);
        nlohmann::json j;
        i >> j;
        Asset asset;
        asset.file_path = world;
        asset.type = "world";
        asset.file_name = j["file_name"].get<std::string>();
        asset.is_favorite = j["is_favorite"].get<bool>();
        asset.is_static = j["is_static"].get<bool>();
        for (const auto& data : j["data"].items()) {
          IData data_item;
          data_item.name = data.key();
          data_item.value = data.value().get<std::string>();
          asset.data[data_item.name] = data_item;
        }

        g_asset_manager->add_asset(asset.file_name, asset);
        g_asset_manager->save_asset(asset.file_name, asset.file_path);
        TabsGenerator::create_asset_tab<WorldTab>(asset.file_name);
        g_editor_manager->get_editor<TabsWindowEditor>()->reload();

        m_worlds[asset.file_name] = asset;
      }
    }
  }
}

void PrefabEditor::handle_click_asset(const ItemAssetType type, const std::unordered_map<std::string, Asset>& assets) {
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      m_item_clicked = type;
      p_mouse_pos = ImGui::GetMousePos();
    }

    for(auto& asset : assets) {
      if(g_editor_manager->get_editor<TabsWindowEditor>()->is_tab_open(asset.second.file_name)){
          ImGui::Indent(10);
          ImGui::Text((asset.second.file_name + " ").c_str());
          ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(245, 173, 100, 35), 0.0f);
          ImGui::Unindent(10);
      }else{
          ImGui::Text(asset.second.file_name.c_str());
      }

      if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()){
        auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab(asset.second.file_name);
        if(tab == nullptr){
          Logger::log(asset.second.file_name + " Tab not found");
          return;
        }

        g_editor_manager->get_editor<TabsWindowEditor>()->open_tab(asset.second.file_name);
        g_editor_manager->get_editor<TabsWindowEditor>()->select_tab(asset.second.file_name);
      }
    }
}

void PrefabEditor::create_asset_popup(const ItemAssetType type) {
  std::string title;
  std::string button_text;
  std::string file_name;
  std::string file_type;
  std::string folder_name;

  switch (type) {
    case PREFAB:
      title = "Create New Prefab";
      button_text = "Create New Prefab";
      file_name = "New Prefab";
      file_type = "prefab";
      folder_name = "prefabs";
      break;
    case DATABASE:
      title = "Create New Database";
      button_text = "Create New Database";
      file_name = "New Database";
      file_type = "database";
      folder_name = "databases";
      break;
    case WORLD:
      title = "Create New World";
      button_text = "Create New World";
      file_name = "New World";
      file_type = "world";
      folder_name = "worlds";
      break;
    default:
      return;
  }

  ImGui::SetNextWindowPos(ImVec2(p_mouse_pos.x, p_mouse_pos.y), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(200, 40), ImGuiCond_Always);
  ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
  if(ImGui::Button(button_text.c_str())){
    Asset* game_profile_asset = g_asset_manager->get_asset("Editor Profile");
    if(game_profile_asset == nullptr){
      Logger::log("Game Profile Asset not found");
      return;
    }

    Asset asset;
    asset.file_name = file_name;
    asset.type = file_type;
    asset.file_path = game_profile_asset->data["folder_path"].value + "\\res\\assets\\" + folder_name + "\\";

    auto name = g_asset_manager->spawn_asset(asset);

    switch (type) {
      case PREFAB:
        //
        break;
      case DATABASE:
        //
        break;
      case WORLD:
        TabsGenerator::create_asset_tab<WorldTab>(name);
        break;
      default:
        return;
    }

    g_editor_manager->get_editor<TabsWindowEditor>()->open_tab(name);
    g_editor_manager->get_editor<TabsWindowEditor>()->select_tab(name);
    reload();
  }
  ImGui::End();
}
