#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonDecoder.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Codable.hpp"

struct B : public Rain::Codable {
    const char *b;

    RAIN_FIELDS_BEGIN(B)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    B()
        : b() {}
};

struct A : public Rain::Codable {
    const char *a;
    B b;

    RAIN_FIELDS_BEGIN(A)
    RAIN_FIELD(a)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    A()
        : a()
        , b() {}
};

struct Car : public Rain::Codable {
    const char *mark;
    int year;
    A a;

    RAIN_FIELDS_BEGIN(Car)
    RAIN_FIELD(mark)
    RAIN_FIELD(year)
    RAIN_FIELD(a)
    RAIN_FIELDS_END

    Car()
        : mark()
        , year()
        , a() {}
};

struct Person : public Rain::Codable {
    std::string name;
    int age;
    std::vector<Car> cars;
    std::vector<int> x;
    const char *surname;

    RAIN_FIELDS_BEGIN(Person)
    RAIN_FIELD(name)
    RAIN_FIELD(age)
    RAIN_FIELD(x)
    RAIN_FIELD(cars)
    RAIN_FIELD(surname)
    RAIN_FIELDS_END

    Person()
        : age()
        , name()
        , surname()
        , cars()
        , x() {}
};

int main() {
    const char *input = R"teststr(
        {
            "name": "Bogdan",
            "age": 10,
            "x": [
                1,
                2,
                3,
                4
            ],
            "cars": [
                {
                    "mark": "Lamborgini",
                    "year": 2020,
                    "a": {
                        "a": "Hello world!",
                        "b": {
                            "b": "123123123"
                        }
                    }
                },
                {
                    "mark": "Lada",
                    "year": 2014,
                    "a": {
                        "a": "Hello world!",
                        "b": {
                            "b": "123123123"
                        }
                    }
                }
            ],
            "surname": "Churban"
        }
    )teststr";

    Person p;
    std::stringstream stream1;
    stream1 << input;
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    decoder->decode(stream1, p);

    std::stringstream stream2;
    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream2, p);
    std::cout << stream2.str() << std::endl;

    return 0;
}