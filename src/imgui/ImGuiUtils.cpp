#include "ImGuiUtils.hpp"

void ImGuiUtils::header_input_text(const std::string& header, std::string* text) {
    ImGui::Text(header.c_str());
    ImGui::SameLine();
    ImGui::InputText(("##" + header).c_str(), text->data(), 256);
    ImGui::Separator();
}
