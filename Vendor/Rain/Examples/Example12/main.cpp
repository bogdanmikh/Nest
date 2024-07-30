#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonDecoder.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Codable.hpp"

struct A : public Rain::Codable {
    std::string text;

    RAIN_FIELDS_BEGIN(A)
    RAIN_FIELD(text)
    RAIN_FIELDS_END

    A()
        : text() {}
};

struct B : public Rain::Codable {
    A a;
    bool x;

    RAIN_FIELDS_BEGIN(B)
    RAIN_FIELD(a)
    RAIN_FIELD(x)
    RAIN_FIELDS_END

    B()
        : a() {}
};

struct C : public Rain::Codable {
    std::vector<B> b;

    RAIN_FIELDS_BEGIN(C)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    C()
        : b() {}
};

int main() {
    const char *input = R"teststr(
{
    "b": [
        {
            "a": {
                "text": "HELLO!"
            },
            "x": true
        }
    ]
}
    )teststr";

    C c;
    std::stringstream stream1;
    stream1 << input;
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    decoder->decode(stream1, c);

    std::stringstream stream2;
    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream2, c);
    std::cout << stream2.str() << std::endl;

    return 0;
}