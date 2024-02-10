#pragma once

#include <filesystem>
namespace amstl {
    static const std::string localPath = std::filesystem::current_path().parent_path().string() + '/';
}