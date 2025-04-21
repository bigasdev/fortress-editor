#pragma once 

#include "IEditor.hpp"
#include <string>
#include <vector>

struct Button {
  std::string name;
  std::string display;
  bool is_pressed = false;
};

class FloatingButtons : public IEditor
{
public:
  FloatingButtons();

  void open() override;
  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;
  void reload() override;

  bool is_pressed(std::string name) {
    for (auto &button : m_buttons) {
      if (button.name == name) {
        return button.is_pressed;
      }
    }
    return false;
  }
private:
  std::vector<Button> m_buttons;
};
