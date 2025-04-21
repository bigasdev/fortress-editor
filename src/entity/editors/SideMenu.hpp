#pragma once 

#include "IEditor.hpp"

class SideMenu : public IEditor{
public:
  SideMenu();

  void open() override;
  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;
  void reload() override;
private: 
};
