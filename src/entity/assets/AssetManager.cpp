#include "AssetManager.hpp"
#include "json.hpp"
#include <fstream>

AssetManager::AssetManager() {
}

void AssetManager::add_asset(const std::string& name, const Asset& asset) {
    m_assets[name] = asset;
}

void AssetManager::save_asset(const std::string& name, const std::string& file_path) {
    auto it = m_assets.find(name);
    if (it != m_assets.end()) {
        // Save the asset data to the specified file path
        nlohmann::json j;
        j["file_name"] = it->second.file_name;
        j["is_favorite"] = it->second.is_favorite;

        for (const auto& data : it->second.data) {
            j["data"][data.first] = data.second.value;
        }

        std::ofstream o(file_path);
        o << std::setw(4) << j << std::endl;
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
