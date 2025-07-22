#pragma once

#include "../tools/Common.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class GridData;

struct Component {
  std::string type;
  std::string value;
  std::string val_1;
  std::string val_2;
};

struct ComponentData {
  std::string name;
  std::vector<std::pair<std::string, Component>> variables;
};

struct PrefabData {
  std::string name;
  std::vector<ComponentData> components;
};

// new system!!! sdl gpu map
struct OptionMenu {
  std::vector<std::string> options;
  vec2 pos;
  bool is_open = false;
};

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

  std::vector<PrefabData> m_prefabs;
  std::unordered_map<std::string, ComponentData> m_components;

  PrefabData *m_current_prefab;

  // new system!!! sdl gpu map
  OptionMenu m_option_menu;
  bool m_is_on_grid = false;
};
