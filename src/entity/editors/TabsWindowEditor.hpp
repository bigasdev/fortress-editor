#pragma once 

#include "IEditor.hpp"
#include "../../entity/tabs/ITab.hpp"
#include <map>
#include <memory>
#include <string>

class TabsWindowEditor : public IEditor
{
public:
    TabsWindowEditor();

    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;

    //tabs 
    void add_tab(const std::string& name, std::shared_ptr<ITab> tab) {
        m_tabs[name] = tab;
    }
    void remove_tab(const std::string& name) {
        m_tabs.erase(name);
    }
    std::shared_ptr<ITab> get_tab(const std::string& name) {
        return m_tabs[name];
    }
    void open_tab(const std::string& name);
    void close_tab(const std::string& name);
    void select_tab(const std::string& name);
    void unselect_tab(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<ITab>> m_tabs;
    std::string m_selected_tab = "";
};
