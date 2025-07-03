#pragma once

#include "../tools/Common.hpp"
#include <string>
#include <vector>

class GPU_Image;

struct GridCell {
  std::string name;
  Rect grid;
  Col color;
  Col m_selected_color;

  int get_x(int pos) const { return (grid.x - pos) / grid.w; }

  int get_y(int pos) const { return (grid.y - pos) / grid.h; }
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
  GridCell *m_selected_cell = nullptr;
  int base_px_w = 8;
  int zoom = 2;

  GPU_Image *m_current_image = nullptr;
};
