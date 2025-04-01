#include "DBView.hpp"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../../tools/Logger.hpp"

DBView::DBView(){
  Table default_table = Table{
    "default",
    {
      {"id", ColumnType::INT},
      {"name", ColumnType::STRING},
      {"position", ColumnType::VEC2},
    },
    {
      {"1", "test", "id"},
      {"2", "test2", "name"},
      {"3", "test3", "position"},
    }
  };
  Table test_table = Table{
    "test",
    {
      {"id", ColumnType::INT},
      {"name", ColumnType::STRING},
      {"position", ColumnType::VEC2},
    }
  };
  Table last_table = Table{
    "last",
    {
    }
  };

  m_tables["default"] = default_table;
  m_tables["test"] = test_table;
  m_tables["last"] = last_table;
}

void DBView::collumn_popup(){
  if(ImGui::BeginPopup("Create Table")){
    static char column_name[128] = "";
    static ColumnType column_type = ColumnType::INT;
    ImGui::InputText("Column Name", column_name, IM_ARRAYSIZE(column_name));
    ImGui::Combo("Column Type", (int*)&column_type, "INT\0STRING\0FLOAT\0BOOL\0ARRAY\0VEC2\0VEC3\0");
    if(ImGui::Button("Create Column", ImVec2(ImGui::GetContentRegionAvail().x - 10, 20))){
      m_selected_table->columns.push_back({column_name, column_type});
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void DBView::item_popup(){
  if(ImGui::BeginPopup("Create Item")){
    static char item_name[128] = "";
    static char item_value[128] = "";
    ImGui::InputText("Item Name", item_name, IM_ARRAYSIZE(item_name));
    ImGui::InputText("Item Value", item_value, IM_ARRAYSIZE(item_value));
    if(ImGui::Button("Create Item", ImVec2(ImGui::GetContentRegionAvail().x - 10, 20))){
      auto item = Item{item_name, item_value, m_selected_column->name};
      m_selected_table->items.push_back(item);
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void DBView::TablesMenu(){
  ImGui::Begin("Table View");
  collumn_popup();
  static char table_search[128] = "";
  ImGui::InputText("##Search", table_search, IM_ARRAYSIZE(table_search));
  for(auto& [name, table] : m_tables){
    if(ImGui::CollapsingHeader(name.c_str())){

      for(auto& column : table.columns){
        ImGui::Text(column.name.c_str());
      }
      if(ImGui::Button("create new colunm", ImVec2(ImGui::GetContentRegionAvail().x - 10, 20))){
        m_selected_table = &table;
        ImGui::OpenPopup("Create Table");
      }
      if(ImGui::Button(("access_" + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 10, 20))){
        table.is_open = true;
      }
    }
    ImGui::Separator();
  }
  ImGui::Separator();
  if (ImGui::Button("Create", ImVec2(ImGui::GetContentRegionAvail().x - 10, 20))) {
    ImGui::OpenPopup("Create Table");
  }
  ImGui::End();
}

void DBView::TablesWindow(){
  for(auto& [name, table] : m_tables){
    if(table.is_open){
      ImGui::Begin((name + "_window").c_str(),  &table.is_open);
      ImGui::BeginTable("table", table.columns.size());
      item_popup();
      for(auto& column : table.columns){
        ImGui::TableSetupColumn(column.name.c_str());
      }
      ImGui::TableHeadersRow();
      for(auto& column : table.columns){
        ImGui::TableNextColumn();
        if(ImGui::Button(("create_" + column.name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 10, 20))){
          m_selected_column = &column;
          m_selected_table = &table;
          ImGui::OpenPopup("Create Item");
        }
        
      }
      for(int i = 0; i < table.columns.size(); i++){
        ImGui::TableNextRow();
        for(auto& item : table.items){
          if(item.column == table.columns[i].name){
            ImGui::TableNextColumn();
            ImGui::Text(table.items[i].value.c_str());
          }

        }
      }
      //the same algiorithm but using the column in the for int 

      ImGui::EndTable();
      ImGui::End();
    }
  }
}

void DBView::show(){
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  TablesMenu();
  TablesWindow();


  ImGui::PopStyleColor();
}

void DBView::update(){
}

void DBView::dispose(){
}

void DBView::draw(){
}
