#include "Rain/Coders/YamlDecoder.hpp"

namespace Rain {

YamlDecoder::YamlDecoder()
    : m_isArray(false)
    , m_document()
    , m_objectStack()
    , m_arrayStack()
    , m_arrayIteratorStack() {}

bool YamlDecoder::decode(std::istream &istream, void *data, TypeInfo info) {
    m_document = YAML::Load(istream);
    info.decoderFunc(nullptr, this, info, data);
    m_objectStack.clear();
    m_arrayIteratorStack.clear();
    m_arrayStack.clear();
    m_isArray = false;
    m_document = YAML::Node();
    return true;
}

bool YamlDecoder::beginObject(const char *key) {
    if (m_isArray) {
        // std::cout << "BEGIN OBJECT IN ARRAY" << std::endl;
        const auto &member = *m_arrayIteratorStack.back();
        m_objectStack.push_back({member, m_isArray});
        m_isArray = false;
    } else {
        if (m_objectStack.empty()) {
            // std::cout << "BEGIN ROOT OBJECT" << std::endl;
            m_objectStack.push_back({m_document, false});
        } else {
            // std::cout << "BEGIN OBJECT BY KEY: " << key << std::endl;
            if (!currentObject()[key]) {
                return false;
            }
            const auto &member = currentObject()[key];
            if (!member.IsMap()) {
                return false;
            }
            m_objectStack.push_back({member, m_isArray});
        }
        m_isArray = false;
    }
    return true;
}

bool YamlDecoder::decode(const char *key, int &data) {
    if (m_isArray) {
        const auto &member = *m_arrayIteratorStack.back();
        data = member.as<int>();
    } else {
        if (!currentObject()[key]) {
            return false;
        }
        const auto &member = currentObject()[key];
        data = member.as<int>();
    }
    return true;
}

bool YamlDecoder::decode(const char *key, float &data) {
    if (m_isArray) {
        const auto &member = *m_arrayIteratorStack.back();
        data = member.as<float>();
    } else {
        if (!currentObject()[key]) {
            return false;
        }
        const auto &member = currentObject()[key];
        data = member.as<float>();
    }
    return true;
}

bool YamlDecoder::decode(const char *key, std::string &data) {
    if (m_isArray) {
        const auto &member = *m_arrayIteratorStack.back();
        data = member.as<std::string>();
    } else {
        if (!currentObject()[key]) {
            return false;
        }
        const auto &member = currentObject()[key];
        data = member.as<std::string>();
    }
    return true;
}

bool YamlDecoder::decode(const char *key, const char *&data) {
    // DECODING OF CONST CHAT NOT ALLOWED
    data = "Decoding of const char not allowed";
    return true;
}

void YamlDecoder::endObject() {
    // std::cout << "END OBJECT" << std::endl;
    m_isArray = m_objectStack.back().isPartOfArray;
    m_objectStack.pop_back();
}

bool YamlDecoder::beginArray(const char *key) {
    // std::cout << "BEGIN ARRAY" << std::endl;
    if (m_objectStack.empty()) {
        if (!m_document.IsSequence()) {
            return false;
        }
        m_arrayStack.push_back({m_document, m_isArray});
        m_arrayIteratorStack.emplace_back(m_document.begin());
    } else {
        if (m_isArray) {
            const auto &member = *m_arrayIteratorStack.back();
            if (!member.IsSequence()) {
                return false;
            }
            m_arrayStack.push_back({member, m_isArray});
            m_arrayIteratorStack.push_back(member.begin());
        } else {
            if (!currentObject()[key]) {
                return false;
            }
            const auto &member = currentObject()[key];
            if (!member.IsSequence()) {
                return false;
            }
            m_arrayStack.push_back({member, m_isArray});
            m_arrayIteratorStack.push_back(member.begin());
        }
    }
    m_isArray = true;
    return true;
}

bool YamlDecoder::arrayHasElement() {
    if (m_arrayStack.empty() || m_arrayIteratorStack.back() == m_arrayStack.back().node.end()) {
        // std::cout << "ARRAY NO NEXT ELEMENT" << std::endl;
        return false;
    }
    // std::cout << "ARRAY NEXT ELEMENT" << std::endl;
    return true;
}

void YamlDecoder::arrayNext() {
    ++m_arrayIteratorStack.back();
}

void YamlDecoder::endArray() {
    // std::cout << "END ARRAY" << std::endl;
    m_isArray = m_arrayStack.back().isPartOfArray;
    m_arrayStack.pop_back();
    m_arrayIteratorStack.pop_back();
}

const YAML::Node &YamlDecoder::currentObject() {
    return m_objectStack.empty() ? m_document : m_objectStack.back().node;
}

} // namespace Rain