#include "AnimationAction.hpp"

#include "../editors/AnimatorView.hpp"
#include "../../renderer/Sprite.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/boxer.h"

AnimationAction::~AnimationAction() {}

void AnimationAction::execute() {
  m_animators[m_animation.name].animations[m_animation.name] = m_animation;
  Logger::log("Adding animation " + m_animation.name);
}

void AnimationAction::undo() {
  for(auto it = m_animators.begin(); it != m_animators.end(); ++it) {
    if((it->first) == m_animation.name) {
      auto selection = boxer::show(("Are you sure you want to undo : " + m_animation.name).c_str(), "Undo", boxer::Style::Warning, boxer::Buttons::YesNo);
      Logger::log("Undoing animation " + m_animation.name);
      if(selection == boxer::Selection::Yes) {
        m_animators.erase(it);
      }
      break;
    }
  }
}

