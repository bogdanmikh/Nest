#include "Nest/SystemTools/SystemTools.hpp"

#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_DESKTOP
#    include <nfd.hpp>
#endif

namespace Nest {

std::optional<Path> SystemTools::openFolderDialog(const char *initialFolder) {
#ifdef PLATFORM_DESKTOP
    NFD::Init();
    nfdu8char_t *outPath;
    nfdresult_t result = NFD::PickFolder(outPath);
    std::optional<std::filesystem::path> res;
    if (result == NFD_OKAY) {
        res = outPath;
        NFD::FreePath(outPath);
    } else if (result == NFD_CANCEL) {
        LOG_INFO("User pressed cancel.");
    } else {
        LOG_ERROR("Error: %s", NFD::GetError());
    }
    NFD::Quit();
    return res;
#endif
    return {};
}

std::optional<Path> SystemTools::openFileDialog(const char *filter) {
    return {};
}

std::optional<Path>
SystemTools::saveFileDialog(const char *filter, const char *defaultPath, const char *defaultName) {
#ifdef PLATFORM_DESKTOP
    NFD::Init();
    nfdu8char_t *outPath;
    nfdresult_t result = NFD::SaveDialog(outPath, nullptr, 0, defaultPath, defaultName);
    std::optional<std::filesystem::path> res;
    if (result == NFD_OKAY) {
        res = outPath;
        NFD::FreePath(outPath);
    } else if (result == NFD_CANCEL) {
        LOG_INFO("User pressed cancel.");
    } else {
        LOG_ERROR("Error: %s", NFD::GetError());
    }
    NFD::Quit();
    return res;
#endif
}

void SystemTools::copyFolder(const Path &sourcePath, const Path &newPath) {
#ifdef PLATFORM_POSIX
    std::string command = "cp -rf " + sourcePath.string() + " " + newPath.string();
    system(command.c_str());
#elif defined(PLATFORM_WINDOWS)
    std::string command = "copy " + sourcePath.string() + " " + newPath.string();
    system(command.c_str());
#else
#    error Unsupported platrofm
#endif
}

void SystemTools::open(const Path &path) {
#ifdef PLATFORM_POSIX
    std::string command = "open \"" + path.string() + "\"";
    system(command.c_str());
#elif defined(PLATFORM_WINDOWS)
    std::string command = "start \"" + path.string() + "\"";
    system(command.c_str());
#else
#    error Unsupported platrofm
#endif
}

void SystemTools::show(const Path &path) {
#ifdef PLATFORM_POSIX
    std::string command = "open -R \"" + path.string() + "\"";
    system(command.c_str());
#elif defined(PLATFORM_WINDOWS)
    std::string command = "explorer /select, \"" + path.string() + "\"";
    system(command.c_str());
#else
#    error Unsupported platrofm
#endif
}

} // namespace Nest