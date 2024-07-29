#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonDecoder.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Codable.hpp"

class B : public Rain::Codable {
public:
    std::string b;

    RAIN_FIELDS_BEGIN(B)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    explicit B(const char *b)
        : b(b) {}

    B()
        : b() {}
};

struct A : public Rain::Codable {
    std::string a;
    B b;

    RAIN_FIELDS_BEGIN(A)
    RAIN_FIELD(a)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    A(const char *a, B b)
        : a(a)
        , b(b) {}

    A()
        : a()
        , b() {}
};

int main() {
    const char *input = R"teststr(
        {
            "a": "123",
            "b": {
                "b": "432"
            }
        }
    )teststr";

    A a;
    std::stringstream stream1;
    stream1 << input;
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    decoder->decode(stream1, a);

    std::stringstream stream2;
    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream2, a);
    std::cout << stream2.str() << std::endl;

    return 0;
}