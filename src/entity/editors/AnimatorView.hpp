#pragma once 

#include "IEditor.hpp"
#include <map>
#include <string>

class Sprite;

struct Animation{
  std::string parent;
  std::string name;
  int x = 0, y = 0, frames = 4;
  bool loop = true;
  bool block_transition = false;
};

struct Animator{
  std::string name;
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

    Animator* m_selected_animator = nullptr;

    std::map<std::string, Animator> m_animators;

    int m_current_frame = 0;
    float m_current_timer = 0;
    float m_frame_time = 0.46f;
};
