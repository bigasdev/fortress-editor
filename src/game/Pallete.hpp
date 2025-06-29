#pragma once

#include <string>
#include <vector>
class Pallete {
public:
  Pallete() = default;
  ~Pallete() = default;

  void init();
  void update();
  void side_draw();
  void draw();
  void clean();

private:
  std::vector<std::string> m_palettes;
  std::string m_current_palette = "";
};
