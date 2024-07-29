#pragma once

#include "Rain/Coder.hpp"

#include <yaml-cpp/yaml.h>

namespace Rain {

class YamlDecoder final : public Decoder {
    struct Object {
        const YAML::Node node;
        bool isPartOfArray;
    };

public:
    YamlDecoder();
    ~YamlDecoder() override = default;

    bool decode(std::istream &istream, void *data, TypeInfo info) override;
    bool beginObject(const char *key) override;
    bool decode(const char *key, int &data) override;
    bool decode(const char *key, float &data) override;
    bool decode(const char *key, std::string &data) override;
    bool decode(const char *key, const char *&data) override;
    void endObject() override;
    bool beginArray(const char *key) override;
    bool arrayHasElement() override;
    void arrayNext() override;
    void endArray() override;

private:
    const YAML::Node &currentObject();
    bool m_isArray;
    YAML::Node m_document;
    std::vector<Object> m_objectStack;
    std::vector<Object> m_arrayStack;
    std::vector<YAML::Node::const_iterator> m_arrayIteratorStack;
};

} // namespace Rain