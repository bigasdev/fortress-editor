#include "Mouse.hpp"
#include "SDL.h"
#include <cmath>

bool Mouse::is_at_area(Rect pArea, int width, int height) {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);
  return x >= pArea.x && x < pArea.x + pArea.w && y >= pArea.y &&
         y < pArea.y + pArea.h;
}

Rect Mouse::get_mouse_area(int width, int height) {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);

  int scaled_x = x * 64 / width;
  int scaled_y = y * 64 / height;

  return Rect(scaled_x - 4, scaled_y - 4, 8, 8);
}

vec2 Mouse::get_mouse_pos() {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);
  return vec2(x, y);
}

float Mouse::get_mouse_angle(vec2 pos) {
  vec2 mousePos = get_mouse_pos();
  return std::atan2(mousePos.y - pos.y, mousePos.x - pos.x);
}
