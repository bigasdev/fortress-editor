#pragma once 
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
class EditorDataManager;
class Fini;

extern Engine* g_engine;
extern EditorDataManager* g_editor_data_manager;
extern Res* g_res;
extern Renderer* g_renderer;
extern Camera* g_camera;
extern Cooldown* g_cooldown;
extern SoundManager* g_sound_manager;
extern InputManager* g_input_manager;
extern UndoManager* g_undo_manager;
extern Fini* g_fini;

// variables 
extern EntityData* g_selected_entity;

extern bool g_controller_connected;
extern bool g_enter_pressed;
extern bool g_ctrl_pressed;
extern bool g_s_pressed;
extern bool g_o_pressed;

#endif
