#include "GameProfileTab.hpp"

#include "../../imgui/imgui_impl_opengl3.h"

#include "../../tools/Logger.hpp"

GameProfileTab::GameProfileTab(const std::string& _name) {
  name = _name;
}

void GameProfileTab::open() {
}

void GameProfileTab::show() {
}

void GameProfileTab::update() {
}

void GameProfileTab::dispose() {
}

void GameProfileTab::draw() {
}

void GameProfileTab::reload() {
}

void GameProfileTab::save() {
  is_dirty = false;
}
