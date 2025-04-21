#include "MainMenu.hpp"
#include "../../imgui/imgui.h"
#include "../../core/global.hpp"
#include "EditorManager.hpp"

MainMenu::MainMenu() {}

void MainMenu::open() {
}

void MainMenu::show() {
  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("New")) {
      // New();
    }
    if (ImGui::MenuItem("Open", "Ctrl+O")) {
      // Open();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {
      // Save();
    }
    if (ImGui::MenuItem("Save As..")) {
      // SaveAs();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Exit")) {
      // Exit();
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Edit")) {
    if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      // Undo();
    }
    if (ImGui::MenuItem("Redo", "CTRL+Y")) {
      // Redo();
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Reload")) {
    if (ImGui::MenuItem("Current Editor")) {
      g_editor_manager->get_selected_editor()->reload();
    }
    if (ImGui::MenuItem("All Editors")) {
      for (auto &editor : g_editor_manager->get_editors()) {
        editor->reload();
      }
    }
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

void MainMenu::update() {}

void MainMenu::dispose() {}

void MainMenu::draw() {}

void MainMenu::reload() {}
