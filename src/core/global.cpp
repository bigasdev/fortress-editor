#include "global.hpp"

Engine* g_engine;
EditorDataManager* g_editor_data_manager;
Res* g_res;
Renderer* g_renderer;
Camera* g_camera;
Cooldown* g_cooldown;
SoundManager* g_sound_manager;
InputManager* g_input_manager;
UndoManager* g_undo_manager;
Fini* g_fini;
FloatingButtons* g_floating_buttons;

EntityData* g_selected_entity;

bool g_controller_connected;
bool g_enter_pressed;
bool g_ctrl_pressed;
bool g_s_pressed;
bool g_o_pressed;
bool g_del_pressed;

