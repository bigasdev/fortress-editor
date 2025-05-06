#include "EditorProfileTab.hpp"

#include "../../tools/Logger.hpp"
#include "../../core/global.hpp"
#include "../../core/EditorDataManager.hpp"
#include "../../imgui/ImGuiUtils.hpp"
#include "TabUtils.hpp"

EditorProfileTab::EditorProfileTab(const std::string& _name) {
  name = _name;
  m_asset = g_asset_manager->get_asset("Editor Profile");
}

void EditorProfileTab::open() {
}

void EditorProfileTab::show() {
  TabUtils::tab("Editor Profile");
}

void EditorProfileTab::update() {
  if(m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }
}

void EditorProfileTab::dispose() {
}

void EditorProfileTab::draw() {
  if(m_asset == nullptr) {
    m_asset = g_asset_manager->get_asset("Editor Profile");
    return;
  }
  TabUtils::asset_header(m_asset);
  ImGuiUtils::header_input_text(" Folder path", &m_asset->data["folder_path"].value);
  ImGuiUtils::header_input_text(" Current path", &m_asset->data["current_path"].value);
}

void EditorProfileTab::reload() {
}

void EditorProfileTab::save() {
  is_dirty = false;
  m_asset->start();
  //g_editor_data_manager->m_current_path = m_current_path_buffer;
  //g_folder_path = m_folder_path_buffer;
}
