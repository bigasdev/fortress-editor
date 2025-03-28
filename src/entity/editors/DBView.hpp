#pragma once

#include "IEditor.hpp"
#include <string>
#include <vector>
#include <map>

enum class ColumnType {
  INT,
  STRING,
  FLOAT,
  BOOL,
  ARRAY,
  VEC2,
  VEC3
};

struct Item {
  std::string name;
  std::string value;
  std::string column;
};

struct Column {
  std::string name;
  ColumnType type;
};

struct Table{
  std::string name;
  std::vector<Column> columns;
  std::vector<Item> items;

  bool is_open = false;
};

class DBView : public IEditor{
public:
  DBView();
  void show() override;
  void update() override;
  void dispose() override;
  void draw() override;

  void TablesMenu();
  void TablesWindow();

private:
  std::map<std::string, Table> m_tables;
};
