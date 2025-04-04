#pragma once

#include <cstdint>
#include <string>

namespace Nest {
extern bool writeBinaryFile(const std::string &filename, const char *data, uint32_t size);
extern bool readBinaryFile(const std::string &filename, char *data, uint32_t size);
} // namespace Nest