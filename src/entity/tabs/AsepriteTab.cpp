#include "AsepriteTab.hpp"

#include "../../imgui/ImGuiUtils.hpp"
#include "../../tools/Logger.hpp"
#include "../../core/Timer.hpp"
#include "../../core/global.hpp"
#include "../../renderer/Renderer.hpp"
#include "../editors/RendererViewer.hpp"
#include "TabUtils.hpp"

AsepriteTab::AsepriteTab(const std::string& _name, const std::string& _file_path, const std::string& _file_name) {
  name = _name;
  m_file_path = _file_path;
  m_file_name = _file_name;
  m_viewer = new RendererViewer();
}
