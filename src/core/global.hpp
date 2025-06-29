#pragma once
#include <string>
#ifndef GLOBAL_HPP
#define GLOBAL_HPP

class Engine;
class Res;
class SoundManager;
class Renderer;
class Camera;
class Cooldown;
class InputManager;
class EntityData;
class EditorManager;
class AssetManager;
class Fini;

extern Engine *g_engine;
extern EditorManager *g_editor_manager;
extern AssetManager *g_asset_manager;
extern Res *g_res;
extern Renderer *g_renderer;
extern Camera *g_camera;
extern Cooldown *g_cooldown;
extern SoundManager *g_sound_manager;
extern InputManager *g_input_manager;
extern Fini *g_fini;

// variables
extern bool g_left_click;
extern bool g_right_click;
extern bool g_middle_click;
extern bool g_controller_connected;

#endif
