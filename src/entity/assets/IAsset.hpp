#pragma once 

#include <string>

struct IData{

};

//datas
struct GameProfileData : public IData{
};

struct EditorProfileData : public IData{
  std::string m_folder_path = "";
  std::string m_current_path = "";
};

struct Asset{
    std::string file_name;
    std::string file_path;
    bool is_static = false;
    bool is_favorite = false;
    bool is_dirty = false;

    IData data = IData{};
};
