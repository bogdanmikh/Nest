#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/YamlEncoder.hpp"
#include "Rain/Coders/YamlDecoder.hpp"
#include "Rain/Codable.hpp"

int main() {
    const char *input = R"teststr(
---
- 1.1
- 2.0
- 3.3
- 4.0
- 5.5
- 6.0
...
    )teststr";

    std::stringstream stream1;
    stream1 << input;
    Rain::Decoder *decoder = new Rain::YamlDecoder;
    std::vector<float> values;
    decoder->decode(stream1, values);

    std::stringstream stream2;
    Rain::Encoder *encoder = new Rain::YamlEncoder;
    encoder->encode(stream2, values);

    std::cout << stream2.str() << std::endl;

    return 0;
}