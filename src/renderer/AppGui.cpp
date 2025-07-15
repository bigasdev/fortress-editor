#if _IMGUI
#include "AppGui.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_sdlrenderer.h"
#include "../imgui/imgui_impl_opengl3.h"

SDL_Window *GUI::gui_window = nullptr;
SDL_GLContext GUI::gui_renderer = nullptr;

ImFont *font = nullptr;

void GUI::setup(SDL_Window *window, SDL_GLContext &renderer) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  // io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

  // styling

  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowRounding = 5.3f;
  style.FrameRounding = 2.3f;
  style.ScrollbarRounding = 0;

  style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.117f, 0.133f, 0.16f, 1.00f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
  // style.Colors[ImGuiCol_Button] = ImVec4(0.184f, 0.2f, 0.243f, 1.00f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.32f, 0.364f, 0.45f, 1.00f);
  // style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.329f, 0.364f,
  // 0.45f, 1.00f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.498f, 0.615f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.498f, 0.615f, 1.00f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] =
      ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

  // light style from Pacôme Danhiez (user itamago)
  // https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
  io.Fonts->AddFontDefault();

  // Add character ranges and merge into main font
  static ImWchar ranges[] = {0x0020, 0x00FF, // Basic Latin + Latin Supplement
                             0x2190, 0x21FF, // Arrows
                             0x25A0, 0x25FF, // Geometric Shapes
                             0xE000, 0xF8FF, // Private Use Area for icons
                             0};
  ImFontConfig config;
  config.MergeMode = true; // adding custom font
  font = io.Fonts->AddFontFromFileTTF("res/font/blueterm.ttf", 12, &config,
                                      ranges);

  // ImGui::StyleColorsDark();

  GUI::gui_window = window;
  GUI::gui_renderer = renderer;

  ImGui_ImplSDL2_InitForOpenGL(window, renderer);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void GUI::event(SDL_Event event) { ImGui_ImplSDL2_ProcessEvent(&event); }

void GUI::draw(std::function<void()> function) {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(GUI::gui_window);
  ImGui::NewFrame();
  ImGui::PushFont(font);

  // add here our GUI render
  {
    if (function)
      function();
    /*bool isOpen = true;
    ImGui::ShowDemoWindow(&isOpen);

    // example of a window
    /*
    ImGui::Begin("Hello World!");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
    ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); ImGui::End();
    */
  }

  // rendering
  ImGui::PopFont();
  ImGui::Render();
  SDL_GL_MakeCurrent(GUI::gui_window, GUI::gui_renderer);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::clean() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

// we will check some stuff so we can focus the keyboard without breaking
// anything else
void GUI::set_focus() {
  if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
    ImGui::SetKeyboardFocusHere(0);
}
#else
#include "AppGui.hpp"

SDL_Window *GUI::gui_window = nullptr;
SDL_GLContext GUI::gui_renderer = nullptr;

void GUI::setup(SDL_Window *window, SDL_GLContext &renderer) {}
void GUI::event(SDL_Event event) {}
void GUI::draw(std::function<void()> function) {}
void GUI::clean() {}
void GUI::set_focus() {}

#endif
