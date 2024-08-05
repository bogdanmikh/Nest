#include "Nest/Files/Files.hpp"

#include <fstream>
namespace Nest {

bool Nest::writeBinaryFile(const std::string &filename, const char *data, uint32_t size) {
    std::ofstream output(filename, std::ios::binary);
    if (!output.is_open())
        return false;
    output.write(data, size);
    output.close();
    return true;
}

bool Nest::readBinaryFile(const std::string &filename, char *data, uint32_t size) {
    std::ifstream output(filename, std::ios::binary);
    if (!output.is_open())
        return false;
    output.read(data, size);
    output.close();
    return true;
}

} // namespace Nest