#include "AssetEditor.hpp"

#include "../../core/Engine.hpp"
#include "../../entity/tabs/TabUtils.hpp"
#include "../../entity/tabs/WorldTab.hpp"
#include "../../entity/tabs/TabsGenerator.hpp"
#include "../../entity/tabs/AsepriteTab.hpp"
#include "../../tools/FUtils.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "json.hpp"
#include "../../tools/Logger.hpp"


AssetEditor::AssetEditor() {
}

void AssetEditor::open() {
    reload();
}

void AssetEditor::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(320, g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin("Assets from project", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

  if(ImGui::CollapsingHeader("  Palletes")){
    for(auto &pallete : m_palletes){
      if(ImGui::Button(pallete.c_str())){
        auto tab = g_editor_manager->get_editor<TabsWindowEditor>()->get_tab(pallete);
        if(tab == nullptr){
          Asset asset;
          asset.file_name = pallete;
          asset.type = "pallete";
          asset.file_path = m_project_folder + "\\res\\assets\\palletes\\" + pallete + ".json";

          IData aseprite_path;
          aseprite_path.name = "aseprite_path";
          aseprite_path.value = m_project_folder + "\\res\\" + pallete + ".aseprite";

          asset.data["aseprite_path"] = aseprite_path;

           g_asset_manager->add_asset(pallete, asset);
           TabsGenerator::create_asset_tab<AsepriteTab>(pallete);
        }

        g_editor_manager->get_editor<TabsWindowEditor>()->open_tab(pallete);
        g_editor_manager->get_editor<TabsWindowEditor>()->select_tab(pallete);
      }
    }
  }

  ImGui::PopStyleColor();
  ImGui::End();
}

void AssetEditor::update() {
}

void AssetEditor::dispose() {
    Logger::log("Asset Editor disposed");
}

void AssetEditor::draw() {
}

void AssetEditor::reload() {
    Logger::log("Reloading Asset Editor");
    m_palletes.clear();
    auto profile = g_asset_manager->get_asset("Editor Profile");
    if (profile == nullptr) {
      Logger::log("Editor Profile Asset not found");
      return;
    }else{
      m_project_folder = profile->data["folder_path"].value;
      auto asset_folder = m_project_folder + "\\res\\";

      if(FUtils::folder_exists(asset_folder)){
        std::vector<std::string> files;
        auto query = FUtils::get_all_files_in_folder(asset_folder, files);

        for (const auto& file : files) {
          if (FUtils::file_exists(file)) {
            auto ext = FUtils::get_file_extension(file, ".aseprite");
            if (ext) {
              Logger::log("Loading asset: " + file);
              auto name = FUtils::get_file_name(file);
              name = name.substr(0, name.find_last_of('.'));
              Logger::log("Asset name: " + name);
              m_palletes.push_back(name);
            }
          }
        }
      }
    }
}
