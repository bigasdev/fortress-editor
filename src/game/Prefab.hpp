#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class GridData;

struct Component {
  std::string type;
  char val[128] = "";
  char val_1[128] = "";
  char val_2[128] = "";
};

struct ComponentData {
  std::string name;
  std::vector<std::pair<std::string, Component>> variables;
};

struct PrefabData {
  std::string name;
  std::vector<ComponentData> components;
};

class Prefab {
public:
  Prefab() = default;
  ~Prefab() = default;

  void init();
  void update();
  void side_draw();
  void draw();
  void clean();

private:
  std::vector<GridData> m_grid_data;

  std::unordered_map<std::string, PrefabData> m_prefabs;
  std::unordered_map<std::string, ComponentData> m_components;

  PrefabData *m_current_prefab;
};
