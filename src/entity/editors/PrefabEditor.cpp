#include "PrefabEditor.hpp"

#include "../../entity/tabs/TabUtils.hpp"
#include "../../entity/tabs/WorldTab.hpp"
#include "../../entity/tabs/TabsGenerator.hpp"
#include <iostream>

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

  switch (m_item_clicked) {
    case NONE:
      break;
    case PREFAB:
      prefab_popup();
      break;
    case DATABASE:
      database_popup();
      break;
    case WORLD:
      world_popup();
      break;
  }

  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.2, 0.9, 1.0));

  if(ImGui::CollapsingHeader(" Prefabs")){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      m_item_clicked = PREFAB;
      p_mouse_pos = ImGui::GetMousePos();
    }
    ImGui::Indent(10);
    ImGui::Text("Prefab Editor");
    ImGui::Unindent(10);
  }
  ImGui::PopStyleColor(5);

  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25, 0.95, 0.45, 1.0));
  if(ImGui::CollapsingHeader(" Database")){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      m_item_clicked = DATABASE;
      p_mouse_pos = ImGui::GetMousePos();
    }
    ImGui::Text("Prefab Editor");
  }
  ImGui::PopStyleColor(5);


  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94, 0.53, 0.26, 1.0));
  if(ImGui::CollapsingHeader(" World")){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      m_item_clicked = WORLD;
      p_mouse_pos = ImGui::GetMousePos();
    }
    ImGui::Text("Prefab Editor");
  }
  ImGui::PopStyleColor(5);
    
  ImGui::PushStyleColor(ImGuiCol_Button , ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered , ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text , ImVec4(0.25, 0.55, 0.94, 1.0));
  if(ImGui::Button(" Editor Profile")){
    auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab("Editor Profile");
    if(tab == nullptr){
      Logger::log("Editor Profile Tab not found");
      return;
    }

    g_editor_manager->get_editor<TabsWindowEditor>()->open_tab("Editor Profile");
    g_editor_manager->get_editor<TabsWindowEditor>()->unselect_tab("Editor Profile");
  }
  if(ImGui::Button(" Game Profile")){
    auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab("Game Profile");
    if(tab == nullptr){
      Logger::log("Game Profile Tab not found");
      return;
    }

    g_editor_manager->get_editor<TabsWindowEditor>()->open_tab("Game Profile");
    g_editor_manager->get_editor<TabsWindowEditor>()->unselect_tab("Game Profile");
  }
  ImGui::PopStyleColor(4);

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
}

//popups 
void PrefabEditor::prefab_popup() {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
  ImGui::Begin("Prefab Popup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Prefab Popup");
  ImGui::End();
}

void PrefabEditor::database_popup() {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
  ImGui::Begin("Database Popup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Database Popup");
  ImGui::End();
}

void PrefabEditor::world_popup() {
  ImGui::SetNextWindowPos(ImVec2(p_mouse_pos.x, p_mouse_pos.y), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(200, 40), ImGuiCond_Always);
  ImGui::Begin("World Popup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
  if(ImGui::Button("Create New World")){
    Asset* game_profile_asset = g_asset_manager->get_asset("Editor Profile");
    if(game_profile_asset == nullptr){
      Logger::log("Game Profile Asset not found");
      return;
    }

    Asset world_asset;
    world_asset.file_name = "New World";
    world_asset.type = "world";
    world_asset.file_path = game_profile_asset->data["folder_path"].value + "\\res\\assets\\worlds\\";

    auto name = g_asset_manager->spawn_asset(world_asset);

    auto world_tab = TabsGenerator::create_asset_tab<WorldTab>(name);
    if(!world_tab) {
      Logger::log("Failed to create World Tab");
      return;
    }

    g_editor_manager->get_editor<TabsWindowEditor>()->open_tab(name);
  }
  ImGui::End();
}
