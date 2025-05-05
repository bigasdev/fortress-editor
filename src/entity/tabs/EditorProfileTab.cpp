#include "EditorProfileTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"
#include "../../tools/Logger.hpp"
#include "TabUtils.hpp"

EditorProfileTab::EditorProfileTab(const std::string& _name) {
  name = _name;
}

void EditorProfileTab::open() {
}

void EditorProfileTab::show() {
  TabUtils::tab("Editor Profile");
}

void EditorProfileTab::update() {
}

void EditorProfileTab::dispose() {
}

void EditorProfileTab::draw() {
  ImGui::Text("Editor Profile Tab");
}

void EditorProfileTab::reload() {
}

void EditorProfileTab::save() {
  is_dirty = false;
}
