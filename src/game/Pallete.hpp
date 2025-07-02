#pragma once

#include "../tools/Common.hpp"
#include <string>
#include <vector>

class GPU_Image;

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

  GPU_Image *m_current_image = nullptr;

  vec2 mouse_pos = {0, 0};
  vec2 mouse_end_pos = {0, 0};
};
