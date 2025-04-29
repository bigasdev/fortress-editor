#include "TabUtils.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../core/global.hpp"
#include "../editors/TabsWindowEditor.hpp"
#include "../editors/EditorManager.hpp"
#include "../../tools/Logger.hpp"

void TabUtils::tab(const std::string& name) {
    ImGui::BeginChild(name.c_str(), ImVec2(150, 30), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::Text(name.c_str());
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()){
      Logger::log("Right click on tab " + name);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::CalcTextSize(name.c_str()).x + 10);
    ImGui::PushID(name.c_str());
    if (ImGui::Button("")) {
        auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab(name);
        if (tab) {
            tab->is_open = false;
            Logger::log("Tab " + name + " is now " + (tab->is_open ? "open" : "closed"));
        } else {
            Logger::log("Tab " + name + " not found");
        }
    }
    ImGui::PopID();
    ImGui::EndChild();
}


