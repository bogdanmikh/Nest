#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Codable.hpp"

struct B : public Rain::Codable {
    const char *b;

    RAIN_FIELDS_BEGIN(B)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    explicit B(const char *b)
        : b(b) {}

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

    A(const char *a, B b)
        : a(a)
        , b(b) {}

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

    Car(const char *mark, int year, A a)
        : mark(mark)
        , year(year)
        , a(a) {}

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

    Person(int age, const std::string &name, const char *surname, std::vector<Car> cars)
        : age(age)
        , name(name)
        , surname(surname)
        , cars(cars)
        , x({1, 2, 3, 4}) {}

    Person()
        : name()
        , age()
        , x()
        , cars()
        , surname() {}
};

int main() {
    B b("123123123");
    A a("Hello world!", b);
    Car car1("Lamborgini", 2020, a);
    Car car2("Lada", 2014, a);
    Person p(10, "Bogdan", "Churban", {car1, car2});

    std::stringstream stream;

    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream, p);
    std::cout << stream.str() << std::endl;

    delete encoder;
    return 0;
}
