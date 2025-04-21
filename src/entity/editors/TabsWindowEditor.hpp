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

private:
    std::map<std::string, std::shared_ptr<ITab>> m_tabs;
};
