#pragma once

#include "IEditor.hpp"
#include <string>

class DBView : public IEditor{
public:
  DBView();
  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;
};
