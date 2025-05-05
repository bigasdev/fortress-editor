#include "EditorProfileTab.hpp"

#include "../../tools/Logger.hpp"
#include "../../core/global.hpp"
#include "../../core/EditorDataManager.hpp"
#include "../../imgui/ImGuiUtils.hpp"
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
  if(ImGui::Button("Save Asset")) {
    save();
  }
  ImGui::SameLine();
  if(ImGui::Button("Open Folder")) {
  }
  ImGuiUtils::header_input_text(" Folder path", &m_folder_path);
  ImGuiUtils::header_input_text(" Current path", &m_current_path);
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
