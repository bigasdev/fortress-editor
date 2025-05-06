#pragma once 

#include "IAsset.hpp"
#include <unordered_map>

class AssetManager
{
  public:
    AssetManager();
    ~AssetManager() = default;

    void update();
    void add_asset(const std::string& name, const Asset& asset);
    void save_asset(const std::string& name, const std::string& file_path);
    void remove_asset(const std::string& name);
    Asset* get_asset(const std::string& name);

  private:
    std::unordered_map<std::string, Asset> m_assets;
};
