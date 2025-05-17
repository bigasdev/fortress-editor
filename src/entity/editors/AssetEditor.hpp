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
};
