#include "TabUtils.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../core/global.hpp"
#include "../editors/TabsWindowEditor.hpp"
#include "../editors/PrefabEditor.hpp"
#include "../../imgui/ImGuiUtils.hpp"
#include "../editors/EditorManager.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/FUtils.hpp"

void TabUtils::tab(const std::string& name) {
    auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab(name);
    if(tab == nullptr) {
        return;
    }
    ImGui::BeginChild(name.c_str(), ImVec2(150, 30), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::Text(name.c_str());
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()){
      g_editor_manager->get_editor<TabsWindowEditor>()->select_tab(name);
    }
    ImGui::SameLine();
    if(tab->is_dirty){
        ImGui::SetCursorPosX(ImGui::CalcTextSize(name.c_str()).x + 5);
        ImGui::Text(" * ");
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::CalcTextSize(name.c_str()).x + 22);
    ImGui::PushID(name.c_str());
    if (ImGui::Button("")) {
        g_editor_manager->get_editor<TabsWindowEditor>()->unselect_tab(name);
        g_editor_manager->get_editor<TabsWindowEditor>()->close_tab(name);
        tab->is_open = false;
        Logger::log("Tab " + name + " is now " + (tab->is_open ? "open" : "closed"));
    }
    ImGui::PopID();
    ImGui::EndChild();
}

void TabUtils::asset_header(Asset* asset) {
    ImGui::Text("");
    ImGui::SameLine();
    if(asset->is_dirty){
      ImGui::PushStyleColor(ImGuiCol_Text, IMRED);
      ImGui::Text("");
      ImGui::SameLine();
      ImGui::PopStyleColor();
    }
    ImGui::Text(("(" + asset->file_name + ".json)").c_str());
    ImGui::SameLine();
    if(ImGui::Button("Save Asset")) {
        g_asset_manager->save_asset(asset->file_name, asset->file_path);
        asset->start();
        asset->is_dirty = false;
    }
    ImGui::SameLine();
    if(!asset->is_static){
      if(ImGui::Button("Rename Asset")) {
      }
      ImGui::SameLine();
      if(ImGui::Button("Delete Asset")) {
        g_asset_manager->remove_asset(asset->file_name);
        g_editor_manager->get_editor<PrefabEditor>()->reload();
      }
      ImGui::SameLine();
    }
    if(ImGui::Button("Open Folder")) {
        FUtils::open_folder(FUtils::remove_filename(asset->file_path));
    }
    ImGui::SameLine();
    if(ImGui::Button("Discard Changes")) {
        asset->discard();
        asset->is_dirty = false;
    }
}

//data 
AssetPrefabData TabUtils::get_asset_data(std::string& data) {
    AssetPrefabData asset_data;
    asset_data.x = 9898;
    asset_data.y = 9898;
    asset_data.w = 9898;
    asset_data.h = 9898;

    size_t pos = 0;
    if((pos = data.find("x: ")) != std::string::npos) {
        data.erase(0, pos + 3);
        size_t end = data.find(";");
        asset_data.x = std::stoi(data.substr(0, end));
    }
    if((pos = data.find("y: ")) != std::string::npos) {
        data.erase(0, pos + 3);
        size_t end = data.find(";");
        asset_data.y = std::stoi(data.substr(0, end));
    }
    if((pos = data.find("w: ")) != std::string::npos) {
        data.erase(0, pos + 3);
        size_t end = data.find(";");
        asset_data.w = std::stoi(data.substr(0, end));
    }
    if((pos = data.find("h: ")) != std::string::npos) {
        data.erase(0, pos + 3);
        size_t end = data.find(";");
        asset_data.h = std::stoi(data.substr(0, end));
    }
    return asset_data;
}


