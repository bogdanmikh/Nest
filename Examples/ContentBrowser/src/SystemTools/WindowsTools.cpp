#include "WindowsTools.hpp"

void WindowsTools::open(const std::string &path) {
    std::string command = "start " + path;
    system(command.c_str());
}
void WindowsTools::copyFolder(const std::string &sourcePath, const std::string &newPath) {
    std::string command = "copy " + sourcePath + " " + newPath;
    system(command.c_str());
}
