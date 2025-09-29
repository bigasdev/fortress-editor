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
  std::vector<GridData> m_grid_data;
};
