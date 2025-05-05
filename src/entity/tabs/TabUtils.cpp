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


