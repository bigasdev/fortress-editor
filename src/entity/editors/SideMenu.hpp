#pragma once 

#include "IEditor.hpp"

enum State{
  PREFAB,
  ASSET,
  ANIMATOR,
  DB,
  NONE
};

class SideMenu : public IEditor{
public:
  SideMenu();

  void open() override;
  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;

  State get_state() { return m_state; }
private: 
  State m_state = PREFAB;
};
