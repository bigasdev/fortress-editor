#include "ResGenerator.hpp"
#include "../tools/Reader.hpp"
#include "json.hpp"
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

std::vector<std::string> load_sounds(std::string path = "res/") {
  std::vector<std::string> sounds_names;
  auto sounds = Reader::get_extension_files(path, ".wav");
  for (auto sound : sounds) {
    auto snd = sound.substr(sound.find_last_of("/\\") + 1);
    snd = snd.substr(0, snd.find_last_of("."));
    sounds_names.push_back(snd);
  }
  return sounds_names;
}

std::vector<std::string> load_data(std::string path) {
  std::vector<std::string> data_names;
  auto files = Reader::get_extension_files(path, ".json");

  for (auto file : files) {
    std::string path = file;
    std::string file_name = path.substr(path.find_last_of("/\\") + 1);
    file_name = file_name.substr(0, file_name.find_last_of("."));

    std::string json = Reader::get_file_contents(file);
    if (json.empty()) {
      continue;
    }

    auto data = nlohmann::json::parse(json);

    for (auto &value : data["data"]) {
      try {
        auto name = value["name"].get<std::string>();
        auto folder = value["folder"].get<std::string>();

        std::string data_name = folder + "/" + name;
        data_names.push_back(data_name);

      } catch (nlohmann::json::exception &e) {
      }
    }
  }
  return data_names;
}

std::vector<std::string> load_sprites(std::string path = "res/sprites") {
  std::vector<std::string> sprite_names;
  auto files = Reader::get_extension_files(path, ".json");

  for (auto file : files) {
    std::string path = file;
    std::string file_name = path.substr(path.find_last_of("/\\") + 1);
    file_name = file_name.substr(0, file_name.find_last_of("."));

    std::string json = Reader::get_file_contents(file);
    if (json.empty()) {
      continue;
    }

    auto prefab = nlohmann::json::parse(json);

    // loop through all the keys in the json array
    // get all of the values from the json and try to create an Sprite from it
    for (auto &[key, value] : prefab.items()) {
      try {
        auto name = value["name"].get<std::string>();
        sprite_names.push_back(name);

      } catch (nlohmann::json::exception &e) {
      }
    }
  }
  return sprite_names;
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

  // start sprites
  auto sprites = load_sprites(project_folder + "/res/sprites");
  content += "namespace sprites {\n";
  for (auto s : sprites) {
    std::cout << "Sprite found: " << s << std::endl;
    content += "inline constexpr const char* " + s + " = \"" + s + "\";\n";
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

  // start data
  auto data = load_data(project_folder + "/res/data");
  content += "namespace data {\n";
  for (auto d : data) {
    std::cout << "Data found: " << d << std::endl;
    auto d_clean = d;
    std::replace(d_clean.begin(), d_clean.end(), '/', '_');
    content +=
        "inline constexpr const char* " + d_clean + " = \"" + d + "\";\n";
  }
  content += "}\n";

  // start sounds
  auto sounds = load_sounds(project_folder + "/res/sounds");
  content += "namespace sounds {\n";
  for (auto s : sounds) {
    std::cout << "Sound found: " << s << std::endl;
    content += "inline constexpr const char* " + s + " = \"" + s + "\";\n";
  }
  content += "}\n";

  // finish
  content += "}\n";

  // write the file
  std::ofstream out(project_folder + "/src/res/autogen_res.hpp");
  out << content;
  out.close();
}
