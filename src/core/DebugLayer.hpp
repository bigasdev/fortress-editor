#pragma once

#include <functional>
#include <string>
#include <unordered_map>
struct ListItem {
  std::string name;

  std::function<void()> on_select;
  std::function<void()> on_delete;
  std::function<void()> on_action;
};

struct List {
  std::string title;
  std::vector<ListItem> *items;
  int selected_index = -1;
  bool open = true;
};

class DebugLayer {
public:
  static void start_debug_layer();
  static void draw_debug_layer();
  static void add_list(const std::string &list_name, const List &list) {
    lists[list_name] = list;
  }
  static void remove_list(const std::string &list_name) {
    lists.erase(list_name);
  }

  static void remove_list_item(const std::string &list_name,
                               const std::string &item_name) {
    if (lists.find(list_name) != lists.end()) {
      auto &items = lists[list_name].items;
      items->erase(std::remove_if(items->begin(), items->end(),
                                  [&](const ListItem &item) {
                                    return item.name == item_name;
                                  }),
                   items->end());
    }
  }
  static std::unordered_map<std::string, List> lists;

private:
  DebugLayer() = delete;
};
