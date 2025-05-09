#pragma once 

#include <string>
#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../entity/editors/EditorManager.hpp"
#include "../../entity/editors/TabsWindowEditor.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/FUtils.hpp"
#include "../../entity/assets/AssetManager.hpp"
#include "../../imgui/imgui_impl_opengl3.h"

class TabUtils
{
    public:
        static void tab(const std::string& name);
        static void asset_header(Asset* asset);
        
};
