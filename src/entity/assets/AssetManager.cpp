#include "AssetManager.hpp"

AssetManager::AssetManager() {
}

void AssetManager::add_asset(const std::string& name, const Asset& asset) {
    m_assets[name] = asset;
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
