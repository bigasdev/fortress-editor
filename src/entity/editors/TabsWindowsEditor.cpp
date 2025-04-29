#include "TabsWindowEditor.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../entity/tabs/BaseTab.hpp"

TabsWindowEditor::TabsWindowEditor() {
}

void TabsWindowEditor::open() {
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->open();
    }
  }
}

void TabsWindowEditor::show() {
  ImGui::Begin("Tabs Window Editor", nullptr);
  ImGui::BeginChild("Tabs", ImVec2(0, 230), true);
  //align them to the left 
  for (auto& tab : m_tabs) {
    if(tab.second->is_open){
      tab.second->show();
    }
    ImGui::SameLine();
  }
  ImGui::EndChild();
  ImGui::End();
}

void TabsWindowEditor::update() {
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->update();
    }
  }
}

void TabsWindowEditor::dispose() {
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->dispose();
    }
  }
}

void TabsWindowEditor::draw() {
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->draw();
    }
  }
}

void TabsWindowEditor::reload() {
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->reload();
    }
  }
}
