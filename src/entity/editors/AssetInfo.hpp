#pragma once 

#include "IEditor.hpp"

class EntityData;

class AssetInfo : public IEditor
{
public:
    AssetInfo(EntityData* entity_data);
    ~AssetInfo();

    void open() override;
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;
private:
    EntityData* m_entity_data;
};
