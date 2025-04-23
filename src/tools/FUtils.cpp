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

bool FUtils::is_corrupted(const std::string &path) {
  return false; 
}
bool FUtils::is_empty(const std::string &path) {
  return false; 
}
