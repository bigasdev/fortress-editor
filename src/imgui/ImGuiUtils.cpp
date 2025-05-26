#include "ImGuiUtils.hpp"
#include "imgui_stdlib.h"

void ImGuiUtils::header_input_text(const std::string& header, std::string* text) {
    ImGui::Text(header.c_str());
    ImGui::SameLine();
    ImGui::InputText(("##" + header).c_str(), text);
    ImGui::Separator();
}

void ImGuiUtils::header_input_int(const std::string& header, int* value) {
    ImGui::Text(header.c_str());
    ImGui::SameLine();
    ImGui::InputInt(("##" + header).c_str(), value);
    ImGui::Separator();
}

void ImGuiUtils::header_input_float(const std::string& header, float* value) {
    ImGui::Text(header.c_str());
    ImGui::SameLine();
    ImGui::InputFloat(("##" + header).c_str(), value);
    ImGui::Separator();
}

void ImGuiUtils::header_input_bool(const std::string& header, bool* value) {
    ImGui::Text(header.c_str());
    ImGui::SameLine();
    ImGui::Checkbox(("##" + header).c_str(), value);
    ImGui::Separator();
}

void ImGuiUtils::image(GPU_TextureHandle texture, vec2i sprite_pos, vec2i sprite_size, int atlas_size, vec2i image_size) {
    ImVec2 uv0 = ImVec2((float)sprite_pos.x / atlas_size, (float)sprite_pos.y / atlas_size);
    ImVec2 uv1 = ImVec2((float)(sprite_pos.x + sprite_size.x) / atlas_size, (float)(sprite_pos.y + sprite_size.y) / atlas_size);
    ImGui::Image((void *)(intptr_t)texture, ImVec2(image_size.x, image_size.y), uv0, uv1);
}
