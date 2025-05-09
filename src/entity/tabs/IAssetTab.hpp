#pragma once 
#include "TabsCommon.hpp"
#include "ITab.hpp"

class IAssetTab : public ITab
{
  public:
    virtual ~IAssetTab() {}
    virtual void add_asset(Asset* asset) = 0;
};
