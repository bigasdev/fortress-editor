#pragma once

#include "IAction.hpp"

#include "../editors/AnimatorView.hpp"
#include <map>
#include <string>

class AnimationAction : public IAction {
public:
  AnimationAction(Animation animation,
                  Animator &selected_animator)
      : m_animation(animation), m_selected_animator(m_selected_animator) {}
  ~AnimationAction();

  void execute() override;
  void undo() override;

private:
  Animation m_animation;
  Animator m_selected_animator;
};
