#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/YamlEncoder.hpp"
#include "Rain/Codable.hpp"

int main() {
    std::vector<float> vec = {1, 2, 3, 4, 5, 6};

    std::stringstream stream;

    Rain::Encoder *encoder = new Rain::YamlEncoder;
    encoder->encode(stream, vec);
    std::cout << stream.str() << std::endl;

    delete encoder;
    return 0;
}
