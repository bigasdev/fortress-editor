#pragma once 

#include "IEditor.hpp"
#include <map>
#include <string>

class Sprite;


class AnimatorView : public IEditor
{
public:
    AnimatorView();
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;

    void assets_child();
    void animator_child();

private:
    std::map<std::string, Sprite> m_sprites;
    Sprite* m_selected_sprite = nullptr;
};
