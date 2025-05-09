#pragma once

#include <string>
#include "../../core/Engine.hpp"
#include "../../core/global.hpp"
#include "../../entity/editors/EditorManager.hpp"
#include "../../entity/editors/TabsWindowEditor.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/FUtils.hpp"
#include <memory>
#include "IAssetTab.hpp"

class TabsGenerator{
  public:
      template<typename T>
      static bool create_asset_tab(const std::string& name){
        auto asset = g_asset_manager->get_asset(name);
        if (asset == nullptr) {
          Logger::log("Asset not found: " + name);
          return false;
        }
        T asset_tab(name);
        IAssetTab* iasset_tab = dynamic_cast<IAssetTab*>(&asset_tab);
        iasset_tab->add_asset(asset);

        g_editor_manager->get_editor<TabsWindowEditor>()->add_tab(name, std::make_shared<T>(asset_tab));
        return true;
      }
};
