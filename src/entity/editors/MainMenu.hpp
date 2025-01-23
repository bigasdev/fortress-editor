#pragma once

#include "IEditor.hpp"

class MainMenu : public IEditor{
public:
  MainMenu();

  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;

private:
};
