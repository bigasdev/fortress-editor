#pragma once

#include "../tools/Common.hpp"
#include <memory>
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
  struct ItemParam {
    virtual ~ItemParam() = default;

    std::string name = "Param";
  };

  struct ItemParamInt : public ItemParam {
    int value;
  };

  struct ItemParamFloat : public ItemParam {
    float value;
  };

  struct ItemParamString : public ItemParam {
    std::string value;
  };

  struct ItemParamBool : public ItemParam {
    bool value;
  };

  struct ItemParamVec2 : public ItemParam {
    vec2 value;
  };

  struct SpriteInfo {
    std::string name;
    std::string pallete;
  };

  struct Item {
    std::string name;
    std::string folder;
    // the name of the sprite, but this can be null
    SpriteInfo sprite;
    std::vector<std::shared_ptr<ItemParam>> params;
  };

  std::vector<std::string> m_folders;
  std::unordered_map<std::string, Item> m_items;
  std::vector<GridData> m_grid_data;

  // cache to create the folder
  std::string m_folder_name_cache;

  // popup
  bool m_create_item_popup = false;

  std::unordered_map<std::string, bool> m_items_open;
};
