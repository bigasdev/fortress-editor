#include "global.hpp"

Engine* g_engine;
EditorManager* g_editor_manager;
AssetManager* g_asset_manager;
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
std::string g_folder_path;
std::string g_editor_folder_path;

bool g_left_click;
bool g_right_click;
bool g_middle_click;
bool g_controller_connected;
bool g_enter_pressed;
bool g_ctrl_pressed;
bool g_s_pressed;
bool g_o_pressed;
bool g_del_pressed;
bool g_plus_pressed;
bool g_minus_pressed;

