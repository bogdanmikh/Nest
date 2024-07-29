#include "LinuxTools.hpp"

void LinuxTools::open(const std::string &path) {
    std::string command = "open " + path;
    system(command.c_str());
}
void LinuxTools::copyFolder(const std::string &sourcePath, const std::string &newPath) {
    std::string command = "cp -rf " + sourcePath + " " + newPath;
    system(command.c_str());
}
