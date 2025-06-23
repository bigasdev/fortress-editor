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
class UndoManager;
class EntityData;
class EditorManager;
class AssetManager;
class EditorDataManager;
class Fini;
class FloatingButtons;

extern Engine *g_engine;
extern EditorManager *g_editor_manager;
extern AssetManager *g_asset_manager;
extern EditorDataManager *g_editor_data_manager;
extern Res *g_res;
extern Renderer *g_renderer;
extern Camera *g_camera;
extern Cooldown *g_cooldown;
extern SoundManager *g_sound_manager;
extern InputManager *g_input_manager;
extern UndoManager *g_undo_manager;
extern Fini *g_fini;
extern FloatingButtons *g_floating_buttons;

// variables
extern EntityData *g_selected_entity;
extern std::string g_folder_path;
extern std::string g_editor_folder_path;

extern bool g_left_click;
extern bool g_right_click;
extern bool g_middle_click;
extern bool g_controller_connected;
extern bool g_enter_pressed;
extern bool g_ctrl_pressed;
extern bool g_s_pressed;
extern bool g_o_pressed;
extern bool g_del_pressed;
extern bool g_plus_pressed;
extern bool g_minus_pressed;

#endif
