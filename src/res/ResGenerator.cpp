#include "ResGenerator.hpp"
#include "../tools/Reader.hpp"
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> load_aseprites(std::string path = "res/") {
  std::vector<std::string> aseprite_names;
  auto aseprites = Reader::get_extension_files(path, ".aseprite");
  for (auto aseprite : aseprites) {
    auto ase = aseprite.substr(aseprite.find_last_of("/\\") + 1);
    ase = ase.substr(0, ase.find_last_of("."));
    aseprite_names.push_back(ase);
  }
  return aseprite_names;
}
std::vector<std::string> load_fonts(std::string path = "res/") {
  std::vector<std::string> fonts_names;
  auto fonts = Reader::get_extension_files(path, ".ttf");
  for (auto font : fonts) {
    auto fnt = font.substr(font.find_last_of("/\\") + 1);
    fnt = fnt.substr(0, fnt.find_last_of("."));
    fnt = fnt.substr(0, fnt.find_last_of("-"));
    fonts_names.push_back(fnt);
  }
  return fonts_names;
}

void res::generate_resources(std::string project_folder) {
  auto ase = load_aseprites(project_folder + "/res");
  std::string content;
  content += "#pragma once\n";
  content += "namespace res {\n";
  // start aseprites
  content += "namespace aseprites {\n";
  for (auto a : ase) {
    std::cout << "Aseprite found: " << a << std::endl;
    content += "inline constexpr const char* " + a + " = \"" + a + "\";\n";
  }
  content += "}\n";

  // start fonts
  auto fonts = load_fonts(project_folder + "/res/font");
  content += "namespace fonts {\n";
  for (auto f : fonts) {
    std::cout << "Font found: " << f << std::endl;
    content += "inline constexpr const char* " + f + " = \"" + f + "\";\n";
  }
  content += "}\n";

  // finish
  content += "}\n";

  // write the file
  std::ofstream out(project_folder + "/src/res/autogen_res.hpp");
  out << content;
  out.close();
}
