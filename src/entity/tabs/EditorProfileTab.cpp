#include "EditorProfileTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../tools/Logger.hpp"
#include "../../core/global.hpp"
#include "../../core/EditorDataManager.hpp"
#include "TabUtils.hpp"

EditorProfileTab::EditorProfileTab(const std::string& _name) {
  name = _name;
  m_current_path.resize(256);
  m_folder_path.resize(256);
  m_current_path_buffer.resize(256);
  m_folder_path_buffer.resize(256);

  m_folder_path = g_folder_path;
  m_current_path = g_editor_data_manager->m_current_path;
  m_folder_path_buffer = m_folder_path;
  m_current_path_buffer = m_current_path;

}

void EditorProfileTab::open() {
}

void EditorProfileTab::show() {
  TabUtils::tab("Editor Profile");
}

void EditorProfileTab::update() {
  if (m_folder_path != m_folder_path_buffer) {
    is_dirty = true;
  }
  if (m_current_path != m_current_path_buffer) {
    is_dirty = true;
  }
}

void EditorProfileTab::dispose() {
}

void EditorProfileTab::draw() {
  ImGui::Text("Editor Profile Tab");
  if(ImGui::Button("Save")) {
    save();
  }
  ImGui::InputText("Folder path", &m_folder_path[0], 256);
  ImGui::InputText("Current path", &m_current_path[0], 256);
}

void EditorProfileTab::reload() {
}

void EditorProfileTab::save() {
  is_dirty = false;
  m_folder_path_buffer = m_folder_path;
  m_current_path_buffer = m_current_path;
  g_editor_data_manager->m_current_path = m_current_path_buffer;
  g_folder_path = m_folder_path_buffer;
}
