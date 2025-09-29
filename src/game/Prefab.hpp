#pragma once

#include "../tools/Common.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class GridData;

class Prefab {
public:
  Prefab() = default;
  ~Prefab() = default;

  void init();
  void update();
  void save();
  void side_draw();
  void draw();
  void clean();

private:
  struct Item {
    std::string name;
    // the name of the sprite, but this can be null
    std::string pallete;
  };

  std::vector<GridData> m_grid_data;
};
