#pragma once 

#include <string>
#include <map>

class EntityData;
class Animator;

class EditorDataManager{
  public:
    EditorDataManager();
    ~EditorDataManager();

    void on_file_drop(std::string path);
    void import(std::map<std::string, EntityData>& assets, std::string path = "");
    void export_(std::map<std::string, EntityData> assets, std::string path = "");
    void auto_save(std::map<std::string, EntityData> assets);

    void export_animators(std::map<std::string, Animator> animators, std::string path = "");
    std::map<std::string, Animator> import_animators(std::string path = "");

private:
    std::string m_current_path;
};
