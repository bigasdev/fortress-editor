#pragma once 

#include "IAssetTab.hpp"
#include "ITab.hpp"
#include <string>

class RendererViewer;

class AsepriteTab : public IAssetTab{
public:
    AsepriteTab(const std::string& _name, const std::string& _file_path, const std::string& _file_name);
    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;
    void save() override;
private:
    std::string m_file_path;
    std::string m_file_name;
    Asset* m_asset = nullptr;
    RendererViewer* m_viewer = nullptr;
};
