#pragma once 

#include <string>
#include <unordered_map>
#include "../../tools/Logger.hpp"

struct IData{
    std::string name;
    std::string value;
    std::string value_buffer;
    std::string value_start;
    std::string type;
};

struct Asset{
    std::string file_name;
    std::string type = "asset";
    std::string file_path;
    bool is_static = false;
    bool is_favorite = false;
    bool is_dirty = false;

    std::unordered_map<std::string, IData> data;
    std::unordered_map<std::string, Asset> children;

    void start(){
        for(auto& data : this->data){
            Logger::log("Starting asset: " + data.second.name + " with value: " + data.second.value);
            data.second.value_buffer = data.second.value;
            data.second.value_start = data.second.value;
        }

        for(auto& child : this->children){
            child.second.start();
        }
    }

    void discard(){
        if(!is_dirty) return;

        for(auto& data : this->data){
            data.second.value = data.second.value_start;
            data.second.value_buffer = data.second.value_start;
            is_dirty = false;
        }

        for(auto& child : this->children){
            child.second.discard();
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

        for(auto& child : this->children){
            child.second.update();
            if(child.second.is_dirty){
                is_dirty = true;
                break;
            }
        }
    }
};
