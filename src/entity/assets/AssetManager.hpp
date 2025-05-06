#pragma once 

#include "IAsset.hpp"
#include <unordered_map>

class AssetManager
{
  public:
    AssetManager();
    ~AssetManager() = default;

    void add_asset(const std::string& name, const Asset& asset);
    void remove_asset(const std::string& name);
    Asset* get_asset(const std::string& name);

  private:
    std::unordered_map<std::string, Asset> m_assets;
};
