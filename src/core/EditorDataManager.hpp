#pragma once 

#include <string>
#include <map>

class EntityData;

class EditorDataManager{
  public:
    EditorDataManager();
    ~EditorDataManager();

    void on_file_drop(std::string path);
    void import(std::map<std::string, EntityData>& assets, std::string path = "");
    void export_(std::map<std::string, EntityData> assets, std::string path = "");

private:
    std::string m_current_path;
};
