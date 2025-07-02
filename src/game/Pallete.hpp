#pragma once

#include "../tools/Common.hpp"
#include <string>
#include <vector>

class GPU_Image;

struct GridCell {
  Rect grid;
  Col color;
  Col m_selected_color;
};

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

  std::unordered_map<vec2, GridCell> m_cells;
  int base_px_w = 8;
  int zoom = 2;

  GPU_Image *m_current_image = nullptr;
};
