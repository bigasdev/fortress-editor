#include "TabsWindowEditor.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../entity/tabs/BaseTab.hpp"
#include "../../core/global.hpp"
#include "../../entity/editors/EditorManager.hpp"
#include "../../entity/tabs/GameProfileTab.hpp"
#include "../../entity/tabs/EditorProfileTab.hpp"
#include "../../entity/tabs/WorldTab.hpp"
#include "Fini.hpp"
#include "../../tools/Logger.hpp"
#include <vector>

Fini* tabs_fini = nullptr;

TabsWindowEditor::TabsWindowEditor() {
  auto game_profile_tab = std::make_shared<GameProfileTab>("Game Profile");
  auto editor_profile_tab = std::make_shared<EditorProfileTab>("Editor Profile");
  auto world_tab = std::make_shared<WorldTab>("World");
  add_tab("Game Profile", game_profile_tab);
  add_tab("Editor Profile", editor_profile_tab);
  add_tab("World", world_tab);

  tabs_fini = new Fini("res/tabs.ini");
  tabs_fini->load();

  for(auto& tab : m_tabs) {
    tabs_fini->initialize_value("tabs", tab.first, false);

    auto is_open = tabs_fini->get_value<bool>("tabs", tab.first);
    if(is_open) {
      tab.second->is_open = true;
      tab.second->open();
    } else {
      tab.second->is_open = false;
    }
  }

  tabs_fini->save();
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
  ImGui::BeginChild("Tabs", ImVec2(0, 530), true);
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
  Logger::log("TabsWindowEditor disposed");
  std::cout << "TabsWindowEditor disposed" << std::endl;
  for (auto& tab : m_tabs) {
    if (tab.second->is_open) {
      tab.second->dispose();
    }
  }
  tabs_fini->save();
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

void TabsWindowEditor::open_tab(const std::string& name) {
    auto tab = m_tabs[name];
    if (tab) {
        tab->is_open = !tab->is_open;
    }
    tabs_fini->set_value("tabs", name, tab->is_open);
}

void TabsWindowEditor::close_tab(const std::string& name) {
    tabs_fini->set_value("tabs", name, false);
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
