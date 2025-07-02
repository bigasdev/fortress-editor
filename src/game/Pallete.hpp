#pragma once

#include "../tools/Common.hpp"
#include <string>
#include <unordered_map>
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

  std::unordered_map<int, GridCell> m_cells = {};

  GPU_Image *m_current_image = nullptr;
};
