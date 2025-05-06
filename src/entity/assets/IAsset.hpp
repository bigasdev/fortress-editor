#pragma once 

#include <string>
#include <unordered_map>

struct IData{
    std::string name;
    std::string value;
    std::string value_buffer;
    std::string value_start;
    std::string type;
};

struct Asset{
    std::string file_name;
    std::string file_path;
    bool is_static = false;
    bool is_favorite = false;
    bool is_dirty = false;

    std::unordered_map<std::string, IData> data;

    void start(){
        for(auto& data : this->data){
            data.second.value_buffer = data.second.value;
            data.second.value_start = data.second.value;
        }
    }

    void discard(){
        if(!is_dirty) return;

        for(auto& data : this->data){
            data.second.value = data.second.value_start;
            data.second.value_buffer = data.second.value_start;
            is_dirty = false;
        }
    }

    void update(){
        for(auto& data : this->data){
            if(data.second.value != data.second.value_buffer){
                is_dirty = true;
                break;
            }else{
                is_dirty = false;
            }
        }
    }
};
