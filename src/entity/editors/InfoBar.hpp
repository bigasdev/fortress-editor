#pragma once 

#include "IEditor.hpp"

// This class will be used to control a floating rect that will have some info like cursor position

class InfoBar : public IEditor{
public:
  InfoBar();

  void open() override;
  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;

private:
};
