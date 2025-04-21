#pragma once 

#include "ITab.hpp"
#include <string>

class BaseTab : public ITab
{
public:
    BaseTab() = default;
    BaseTab(const std::string& name) : m_name(name) {}

    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
    void reload() override;
    void save() override;

private:
    std::string m_name;
};
