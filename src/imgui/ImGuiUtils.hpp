#pragma once

#include <string>
#include "imgui_impl_opengl3.h"

#define IMRED ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
#define IMGREEN ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
#define IMBLUE ImVec4(0.0f, 0.0f, 1.0f, 1.0f)
#define IMYELLOW ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
#define IMWHITE ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
#define IMBLACK ImVec4(0.0f, 0.0f, 0.0f, 1.0f)

class ImGuiUtils
{
  public:

      static void header_input_text(const std::string& header, std::string* text);
};
