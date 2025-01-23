#pragma once 

#include "IEditor.hpp"

enum State{
  ASSET,
  NONE
};

class SideMenu : public IEditor{
public:
  SideMenu();

  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;

  State get_state() { return m_state; }
private: 
  State m_state = ASSET;
};
