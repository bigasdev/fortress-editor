#pragma once 

#include "IEditor.hpp"

#include <map>
#include <string>
#include <vector>
class Sprite;
class ase_t;

struct Pallete{
  Sprite *sprite;
  ase_t *ase;
};

class AssetView : public IEditor
{
public:
    AssetView(std::map<std::string, Sprite> sprites, std::string project_folder = "");

    void show() override;
    void update() override;
    void dispose() override;
    void update_sprite_map(std::string project_folder, std::map<std::string, Sprite> sprites);
private:
    void entities();
    void atlas();
    void pallete();

    std::vector<std::string> m_groups;
};
