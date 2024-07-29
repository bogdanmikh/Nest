#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Coders/JsonDecoder.hpp"
#include "Rain/Codable.hpp"

int main() {
    const char *input = R"teststr(
        [
            1.0,
            2.0,
            3.0,
            4.0,
            5.0,
            6.0,
            12.0
        ]
    )teststr";

    std::stringstream stream1;
    stream1 << input;
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    std::vector<float> values;
    decoder->decode(stream1, values);

    std::stringstream stream2;
    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream2, values);

    std::cout << stream2.str() << std::endl;

    return 0;
}