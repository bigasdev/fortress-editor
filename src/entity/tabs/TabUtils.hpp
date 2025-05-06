#pragma once 

#include <string>

class Asset;

class TabUtils
{
    public:
        static void tab(const std::string& name);
        static void asset_header(Asset* asset);
};
