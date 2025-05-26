#include "AsepriteTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"
#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../game/Game.hpp"
#include "../../res/Res.hpp"
#include "../../renderer/Camera.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "../editors/modules/AsepriteViewer.hpp"
#include "TabUtils.hpp"
#include <cstddef>
#include <memory>
#include "SDL_gpu.h"
#include "../../imgui/imgui_stdlib.h"
#include "SDL.h"

AsepriteTab::AsepriteTab(const std::string& _name) {
  name = _name;
  m_viewer = new RendererViewer();
}

void AsepriteTab::open() {
  reload();
}

void AsepriteTab::show() {
  TabUtils::tab(name);
}

void AsepriteTab::update() {
  if (m_asset != nullptr) {
    is_dirty = m_asset->is_dirty;
  }

  m_viewer->update();
  g_camera->set_zoom(3);
}

void AsepriteTab::dispose() {
  g_engine->get_game()->m_viewers.erase(name);
}

void AsepriteTab::draw() {
  //renderer + imgui windows
  if (m_asset != nullptr) {
    TabUtils::asset_header(m_asset);
  }
  if(!m_asset->data["x"].value.empty()){
    ImGui::BeginChild("##Asset info", ImVec2(240, 430), true, ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Current asset : ");
    ImGuiUtils::header_input_text("X", &m_asset->data["x"].value);
    ImGuiUtils::header_input_text("Y", &m_asset->data["y"].value);
    ImGuiUtils::header_input_text("W", &m_asset->data["w"].value);
    ImGuiUtils::header_input_text("H", &m_asset->data["h"].value);
    ImGuiUtils::header_input_text("Name", &m_asset->data["name"].value);
    if(ImGui::Button("Create Asset")){
        std::string asset = "x: " + m_asset->data["x"].value + ";" + "y: " + m_asset->data["y"].value + ";" + "w: " + m_asset->data["w"].value + ";" + "h: " + m_asset->data["h"].value + ";";
        m_asset->data[m_asset->data["name"].value].value = asset; 

        //cleaning the typed data
        m_asset->data["x"].value = "0";
        m_asset->data["y"].value = "0";
        m_asset->data["w"].value = "0";
        m_asset->data["h"].value = "0";
        m_asset->data["name"].value = "none";
    }
    ImGui::SeparatorText("Assets");
    for(auto& data : m_asset_data) {
      if(ImGui::CollapsingHeader(data.first.c_str())) {
        ImGuiUtils::image(GPU_GetTextureHandle(m_ase), {data.second.x, data.second.y}, {data.second.w, data.second.h}, m_ase->texture_w, {32, 32});
        ImGuiUtils::header_input_text("Name", &data.second.name);
        ImGuiUtils::header_input_int("X", &data.second.x);
        ImGuiUtils::header_input_int("Y", &data.second.y);
        ImGuiUtils::header_input_int("W", &data.second.w);
        ImGuiUtils::header_input_int("H", &data.second.h);

        for(auto& asset_data : m_asset->data) {
          if(asset_data.first == data.second.name) {
            asset_data.second.value = "x: " + std::to_string(data.second.x) + ";y: " + std::to_string(data.second.y) + ";w: " + std::to_string(data.second.w) + ";h: " + std::to_string(data.second.h) + ";";
          }
        }
      }
    }
    ImGui::EndChild();
  }
  ImGui::SameLine();
  ImGui::BeginChild("Aseprite Tab", ImVec2(640, 660), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  auto size = ImGui::GetWindowSize();
  auto pos = ImGui::GetWindowPos();


  GPU_SetActiveTarget(g_engine->get_gpu());
  GPU_SetCamera(g_engine->get_gpu(), nullptr);
  m_viewer->draw({size.x - 16, size.y - 16}, {pos.x + 8, pos.y + 8});
  GPU_SetActiveTarget(nullptr);
  GPU_DeactivateShaderProgram();
  ImGui::EndChild();



}

void AsepriteTab::reload() {
  //load the aseprite file
  if (m_asset != nullptr) {
    m_asset->start();
    m_file_path = m_asset->data["aseprite_path"].value;

    m_ase = g_res->load_aseprite(m_file_path.c_str());
    if(m_ase){
      Logger::log("Loaded aseprite: " + m_file_path);
      m_asset->data["aseprite_file"].value = FUtils::get_file_name(m_file_path).substr(0, FUtils::get_file_name(m_file_path).find_last_of('.'));
    }

    m_aseprite_viewer = std::make_shared<AsepriteViewer>(m_ase, m_asset);

    //data loading 
    //the data looks like this: "Cube": "name: Cube;x: 37;y: 6;w: 8;h: 9;"
    for(auto const& data : m_asset->data){
      auto it = data.second.value.find(";");
      if(it == std::string::npos) continue;
      std::string name = data.first;
      std::string value = data.second.value;
      int x,y,w,h;

      size_t pos = 0;
      if((pos = value.find("x: ")) != std::string::npos) {
        value.erase(0, pos + 3);
        size_t end = value.find(";");
        x = std::stoi(value.substr(0, end));
      }
      if((pos = value.find("y: ")) != std::string::npos) {
        value.erase(0, pos + 3);
        size_t end = value.find(";");
        y = std::stoi(value.substr(0, end));
      }
      if((pos = value.find("w: ")) != std::string::npos) {
        value.erase(0, pos + 3);
        size_t end = value.find(";");
        w = std::stoi(value.substr(0, end));
      }
      if((pos = value.find("h: ")) != std::string::npos) {
        value.erase(0, pos + 3);
        size_t end = value.find(";");
        h = std::stoi(value.substr(0, end));
      }

      m_asset_data[name] = {name, x, y, w, h};

      m_aseprite_viewer->add_data(name, &m_asset_data[name]);
    }

    g_engine->get_game()->m_viewers[name] = m_aseprite_viewer;
    m_viewer->add_module(m_aseprite_viewer);
  }
}

void AsepriteTab::save() {
  is_dirty = false;
  m_asset->start();
  //g_editor_data_manager->m_current_path = m_current_path_buffer;
  //g_folder_path = m_folder_path_buffer;
}

void AsepriteTab::add_asset(Asset* asset) {
  Logger::log("Setting asset: " + asset->file_name);
  m_asset = asset;
}
