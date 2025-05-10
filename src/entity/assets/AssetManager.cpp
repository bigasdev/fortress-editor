#include "AssetManager.hpp"
#include "json.hpp"
#include <fstream>
#include "../../tools/FUtils.hpp"
#include "../../tools/Logger.hpp"
#include "../../core/global.hpp"

AssetManager::AssetManager() {
  if(FUtils::folder_exists(g_editor_folder_path + "/res/assets")){
    auto files = FUtils::get_files_in_folder(g_editor_folder_path + "/res/assets");
    for (const auto& file : files) {
      if (FUtils::file_exists(file)) {
        std::ifstream i(file);
        nlohmann::json j;
        i >> j;
        Asset asset;
        asset.file_path = file;
        asset.file_name = j["file_name"].get<std::string>();
        asset.is_favorite = j["is_favorite"].get<bool>();
        asset.is_static = j["is_static"].get<bool>();
        for (const auto& data : j["data"].items()) {
          IData data_item;
          data_item.name = data.key();
          data_item.value = data.value().get<std::string>();
          asset.data[data_item.name] = data_item;
        }
        m_assets[asset.file_name] = asset;
      }
    }
  }

  for (auto& asset : m_assets) {
    asset.second.start();
  }
}

void AssetManager::update() {
  for (auto& asset : m_assets) {
    asset.second.update();
  }
}

void AssetManager::add_asset(const std::string& name, const Asset& asset) {
    if (m_assets.find(name) != m_assets.end()) {
        Logger::error("Asset with name " + name + " already exists.");
        return;
    }

    m_assets[name] = asset;
    save_asset(name, asset.file_path);
}

void AssetManager::save_asset(const std::string& name, const std::string& file_path) {
    auto it = m_assets.find(name);
    if (it != m_assets.end()) {
        // Save the asset data to the specified file path
        nlohmann::json j;
        j["file_name"] = it->second.file_name;
        j["is_favorite"] = it->second.is_favorite;
        j["is_static"] = it->second.is_static;

        for (const auto& data : it->second.data) {
            j["data"][data.first] = data.second.value;
        }

        Logger::log("Saving asset " + name + " to " + file_path);

        std::ofstream o(file_path);
        o << std::setw(4) << j << std::endl;
        o.close();
    }
    else {
        Logger::error("Asset with name " + name + " not found.");
    }
}

void AssetManager::rename_asset(const std::string& name, const std::string& new_name) {
    auto it = m_assets.find(name);
    if (it != m_assets.end()) {
        Asset asset = it->second;
        asset.file_name = new_name;
        m_assets.erase(it);
        m_assets[new_name] = asset;
    }
    else {
        Logger::error("Asset with name " + name + " not found.");
    }
}

void AssetManager::remove_asset(const std::string& name) {
    m_assets.erase(name);
}

Asset* AssetManager::get_asset(const std::string& name) {
    auto it = m_assets.find(name);
    if (it != m_assets.end()) {
        return &it->second;
    }
    return nullptr;
}
