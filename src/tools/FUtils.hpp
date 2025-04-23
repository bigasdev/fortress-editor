#pragma once 

//this will be used to create any util that i might need for the fortress editor, such as check if a folder/file exists
//check if something can be saved, if something is corrupted etc 

#include <string>

class FUtils
{
    public:
        static bool file_exists(const std::string& path);
        static bool folder_exists(const std::string& path);
        static bool is_corrupted(const std::string& path);
        static bool is_empty(const std::string& path);
};
