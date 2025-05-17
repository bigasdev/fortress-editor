#pragma once 

#include "IEditor.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class Asset;

class AssetEditor : public IEditor {
public:
    AssetEditor();
    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;

private:
    std::vector<std::string> m_palletes;
    std::string m_project_folder;
    std::unordered_map<std::string, Asset> m_assets;
};
