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
  ImGui::NewLine();
  if(m_selected_tab != ""){
    ImGui::BeginChild("Tab", ImVec2(0, 0), true);
    auto tab = m_tabs[m_selected_tab];

    if(tab){
      tab->draw();
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();
  ImGui::End();
}

void TabsWindowEditor::update() {
  if(m_selected_tab != ""){
    auto tab = m_tabs[m_selected_tab];

    if(tab){
      tab->update();
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
}

void TabsWindowEditor::reload() {
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->reload();
    }
  }
}

void TabsWindowEditor::select_tab(const std::string& name) {
  m_selected_tab = name;
}

//checks if the open tab is with the same name so we can close it 
void TabsWindowEditor::unselect_tab(const std::string& name) {
  if (m_selected_tab == name) {
    m_selected_tab = "";
  }
}
