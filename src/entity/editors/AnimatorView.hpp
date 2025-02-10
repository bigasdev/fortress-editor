#pragma once 

#include "IEditor.hpp"
#include <map>
#include <string>

class Sprite;

struct Animation{
  std::string name;
  int x, y, frames = 4;
  bool loop = true;
  bool block_transition = false;
};

struct Animator{
  Sprite* sprite = nullptr;

  std::map<std::string, Animation> animations;
};


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

    Animator get_animator(std::string name);

private:
    std::map<std::string, Sprite> m_sprites;

    Animator m_selected_animator;

    std::map<std::string, Animator> m_animators;
};
