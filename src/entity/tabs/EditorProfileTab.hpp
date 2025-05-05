#pragma once 

#include "ITab.hpp"
#include <string>

class EditorProfileTab : public ITab
{
public:
    EditorProfileTab() = default;
    EditorProfileTab(const std::string& _name);

    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;
    void save() override;

private:
    std::string m_folder_path = "";
    std::string m_current_path = "";
    std::string m_folder_path_buffer = "";
    std::string m_current_path_buffer = "";
};
