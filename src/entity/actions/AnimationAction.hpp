#pragma once

#include "IAction.hpp"

#include "../editors/AnimatorView.hpp"
#include <map>
#include <string>

class AnimationAction : public IAction {
public:
  AnimationAction(Animation animation,
                  std::map<std::string, Animator> &animators)
      : m_animation(animation), m_animators(animators) {}
  ~AnimationAction();

  void execute() override;
  void undo() override;

private:
  Animation m_animation;
  std::map<std::string, Animator> &m_animators;
};
