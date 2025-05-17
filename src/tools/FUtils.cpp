#include "FUtils.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

bool FUtils::file_exists(const std::string &path) {
  std::ifstream file(path);
  if (file) {
    return true;
  } else {
    return false;
  }
  file.close();
}

bool FUtils::folder_exists(const std::string &path) {
  return std::filesystem::exists(path) &&
         std::filesystem::is_directory(path);
}

void FUtils::open_folder(const std::string &path) {
  //std::string command = "xdg-open \"" + path + "\""; // For Linux
  std::string command = "explorer \"" + path + "\""; // For Windows
  std::cout << "Opening folder: " << path << std::endl;
  // std::string command = "open \"" + path + "\""; // For macOS
  system(command.c_str());
}

std::vector<std::string> FUtils::get_all_files_in_folder(const std::string &path, std::vector<std::string> &files) {
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if(entry.is_directory()){
      get_all_files_in_folder(entry.path().string(), files);
    }
    files.push_back(entry.path().string());
  }
  return files;
}

std::string FUtils::remove_filename(const std::string &path) {
  return std ::filesystem::path(path).parent_path().string();
}

std::string FUtils::get_file_name(const std::string &path) {
  return std::filesystem::path(path).filename().string();
}

bool FUtils::get_file_extension(const std::string &path, const std::string &ext) {
  std::string extension = std::filesystem::path(path).extension().string();
  return extension == ext;
}

bool FUtils::is_corrupted(const std::string &path) {
  return false; 
}
bool FUtils::is_empty(const std::string &path) {
  return false; 
}

std::string FUtils::get_current_path() {
  return std::filesystem::current_path().string();
}

std::vector<std::string> FUtils::get_files_in_folder(const std::string &path) {
  std::vector<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      files.push_back(entry.path().string());
    }
  }
  return files;
}
