#include "AnimationAction.hpp"

#include "../../renderer/Sprite.hpp"
#include "../../tools/Logger.hpp"
#include "../../tools/boxer.h"
#include "../editors/AnimatorView.hpp"

AnimationAction::~AnimationAction() {}

void AnimationAction::execute() {
  m_selected_animator.animations[m_animation.name] = m_animation;

  Logger::log("Adding animation " + m_animation.name);
}

void AnimationAction::undo() {
  auto selection = boxer::show(
      ("Are you sure you want to undo : " + m_animation.name).c_str(), "Undo",
      boxer::Style::Warning, boxer::Buttons::YesNo);
  Logger::log("Undoing animation " + m_animation.name);
  if (selection == boxer::Selection::Yes and m_selected_animator.animations.find(m_animation.name) != m_selected_animator.animations.end()) {
    m_selected_animator.animations.erase(m_animation.name);
  }
}
