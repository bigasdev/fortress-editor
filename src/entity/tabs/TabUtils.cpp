#include "TabUtils.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../core/global.hpp"
#include "../editors/TabsWindowEditor.hpp"
#include "../editors/EditorManager.hpp"
#include "../../tools/Logger.hpp"

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
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.2, 0.9, 1.0));
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
    if(ImGui::Button("Delete Asset")) {
        g_asset_manager->remove_asset(asset->file_name);
    }
    ImGui::SameLine();
    if(ImGui::Button("Open Folder")) {
        //FUtils::open_folder(asset->file_path);
    }
    ImGui::SameLine();
    if(ImGui::Button("Discard Changes")) {
        asset->discard();
        asset->is_dirty = false;
    }
}


