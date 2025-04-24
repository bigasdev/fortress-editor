#include "PrefabEditor.hpp"

#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../entity/editors/EditorManager.hpp"
#include "../../entity/editors/TabsWindowEditor.hpp"
#include "../../tools/Logger.hpp"
#include "../../imgui/imgui_impl_opengl3.h"
#include <iostream>

bool test = false;
ImVec2 p_mouse_pos = ImVec2(0, 0);

PrefabEditor::PrefabEditor() {
}

void PrefabEditor::open() {
}

void PrefabEditor::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

  if(test){
    ImGui::SetNextWindowPos(p_mouse_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
    ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Test Window");
    ImGui::End();
  }

  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.2, 0.9, 1.0));

  if(ImGui::CollapsingHeader(" Prefabs")){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      test = !test;
      p_mouse_pos = ImGui::GetMousePos();
    }
    ImGui::Indent(10);
    ImGui::Text("Prefab Editor");
    ImGui::Unindent(10);
  }
  ImGui::PopStyleColor(5);

  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25, 0.95, 0.45, 1.0));
  if(ImGui::CollapsingHeader(" Database")){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      Logger::log("Right Clicked Prefab Editor");
    }
    ImGui::Text("Prefab Editor");
  }
  ImGui::PopStyleColor(5);


  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94, 0.53, 0.26, 1.0));
  if(ImGui::CollapsingHeader(" World")){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
      Logger::log("Right Clicked Prefab Editor");
    }
    ImGui::Text("Prefab Editor");
  }
  ImGui::PopStyleColor(5);
    
  ImGui::PushStyleColor(ImGuiCol_Button , ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered , ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::PushStyleColor(ImGuiCol_Text , ImVec4(0.25, 0.55, 0.94, 1.0));
  if(ImGui::Button(" Game Profile")){

  }
  ImGui::PopStyleColor(4);


  if(ImGui::Button("Test Button")){
    g_editor_manager->get_editor<TabsWindowEditor>()->get_tab("Asset")->is_open = true;
  }


  ImGui::PopStyleColor();
  ImGui::End();
}

void PrefabEditor::update(){

}

void PrefabEditor::dispose() {
}

void PrefabEditor::draw() {
}

void PrefabEditor::reload() {
  Logger::log("Reloading Prefab Editor");
}
