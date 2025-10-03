#include "DebugLayer.hpp"
#include "../imgui/ImGuiUtils.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include <iostream>

std::unordered_map<std::string, List> DebugLayer::lists;

void DebugLayer::start_debug_layer() {
  std::cout << "Debug Layer started" << std::endl;
}

void DebugLayer::draw_debug_layer() {
  ImGui::Begin("Debug Layer");

  for (auto &list : lists) {
    if (ImGui::Button(list.second.title.c_str())) {
      list.second.open = !list.second.open;
    }
    ImGui::SameLine();
  }
  ImGui::NewLine();
  ImGui::Separator();

  for (auto &list : lists) {
    if (list.second.open) {
      ImGui::BeginChild(list.first.c_str(), ImVec2(0, 200), true);
      ImGui::Text("%s", list.second.title.c_str());
      ImGui::Separator();
      if (list.second.items) {
        for (int i = 0; i < list.second.items->size(); i++) {
          auto &item = (*list.second.items)[i];
          bool is_selected = (list.second.selected_index == i);
          if (ImGui::Selectable(item.name.c_str(), is_selected)) {
            list.second.selected_index = i;
            if (item.on_select) {
              item.on_select();
            }
          }
          if (is_selected && item.on_action) {
            item.on_action();
          }
          ImGui::SameLine();
          if (ImGui::Button(("X##" + item.name).c_str())) {
            if (item.on_delete) {
              item.on_delete();
            }
            list.second.items->erase(list.second.items->begin() + i);
            if (list.second.selected_index == i) {
              list.second.selected_index = -1;
            } else if (list.second.selected_index > i) {
              list.second.selected_index--;
            }
            i--;
          }
        }
      } else {
        ImGui::Text("No items");
      }
      ImGui::EndChild();
    }
  }

  ImGui::End();
}
