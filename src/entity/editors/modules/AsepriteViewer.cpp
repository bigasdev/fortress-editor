#include "AsepriteViewer.hpp"

#include "../../../core/global.hpp"
#include "../../../renderer/Renderer.hpp"
#include "../../../core/InputManager.hpp"
#include "../../../tools/Logger.hpp"
#include "../../../res/Res.hpp"
#include "../../../tools/Mouse.hpp"
#include "../../../core/Timer.hpp"

AsepriteViewer::AsepriteViewer(GPU_Image* ase) : m_ase(ase) {
}

void AsepriteViewer::update() {
}

void AsepriteViewer::draw(const vec2& pos) {
  g_renderer->draw_raw_sheet(m_ase, pos);
}
