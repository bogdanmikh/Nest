#include "Rain/Coders/JsonDecoder.hpp"
#include "Rain/TypeRegistry.hpp"

namespace Rain {

JsonDecoder::JsonDecoder()
    : m_isArray(false)
    , m_document()
    , m_objectStack()
    , m_arrayStack()
    , m_arrayIteratorStack()
    , m_allocator(m_document.GetAllocator()) {}

bool JsonDecoder::decode(std::istream &istream, void *data, TypeInfo info) {
    rapidjson::IStreamWrapper streamWrapper(istream);
    m_document.ParseStream(streamWrapper);
    if (m_document.HasParseError()) {
        // std::cout << "PARSE ERRORS" << std::endl;
    }
    info.decoderFunc(nullptr, this, info, data);
    m_objectStack.clear();
    m_arrayIteratorStack.clear();
    m_arrayStack.clear();
    m_isArray = false;
    if (m_document.IsArray()) {
        m_document.Clear();
    } else {
        m_document.RemoveAllMembers();
    }
    return true;
}

bool JsonDecoder::beginObject(const char *key) {
    if (m_isArray) {
        // std::cout << "BEGIN OBJECT IN ARRAY" << std::endl;
        const auto &member = *m_arrayIteratorStack.back();
        m_objectStack.push_back({&member, m_isArray});
        m_isArray = false;
    } else {
        if (m_objectStack.empty()) {
            // std::cout << "BEGIN ROOT OBJECT" << std::endl;
            m_objectStack.push_back({&m_document, false});
        } else {
            // std::cout << "BEGIN OBJECT BY KEY: " << key << std::endl;
            if (!currentObject().HasMember(key)) {
                return false;
            }
            const auto &member = currentObject()[key];
            if (!member.IsObject()) {
                return false;
            }
            m_objectStack.push_back({&member, m_isArray});
        }
        m_isArray = false;
    }
    return true;
}

bool JsonDecoder::decode(const char *key, int &data) {
    if (m_isArray) {
        const auto &member = *m_arrayIteratorStack.back();
        if (!member.IsInt()) {
            return false;
        }
        data = member.GetInt();
    } else {
        if (!currentObject().HasMember(key)) {
            return false;
        }
        const auto &member = currentObject()[key];
        if (!member.IsInt()) {
            return false;
        }
        data = member.GetInt();
    }
    return true;
}

bool JsonDecoder::decode(const char *key, float &data) {
    if (m_isArray) {
        const auto &member = *m_arrayIteratorStack.back();
        if (!member.IsFloat()) {
            return false;
        }
        data = member.GetFloat();
    } else {
        if (!currentObject().HasMember(key)) {
            return false;
        }
        const auto &member = currentObject()[key];
        if (!member.IsFloat()) {
            return false;
        }
        data = member.GetFloat();
    }
    return true;
}

bool JsonDecoder::decode(const char *key, std::string &data) {
    if (m_isArray) {
        const auto &member = *m_arrayIteratorStack.back();
        if (!member.IsString()) {
            return false;
        }
        data = member.GetString();
    } else {
        if (!currentObject().HasMember(key)) {
            return false;
        }
        const auto &member = currentObject()[key];
        if (!member.IsString()) {
            return false;
        }
        data = member.GetString();
    }
    return true;
}

bool JsonDecoder::decode(const char *key, const char *&data) {
    // DECODING OF CONST CHAT NOT ALLOWED
    data = "Decoding of const char not allowed";
    return true;
}

void JsonDecoder::endObject() {
    // std::cout << "END OBJECT" << std::endl;
    // assert(!m_objectStack.empty());
    m_isArray = m_objectStack.back().isPartOfArray;
    m_objectStack.pop_back();
}

bool JsonDecoder::beginArray(const char *key) {
    // if (key) {
    //     std::cout << "BEGIN ARRAY " << key << std::endl;
    // } else {
    //     std::cout << "BEGIN ARRAY" << std::endl;
    // }
    if (m_objectStack.empty()) {
        if (!m_document.IsArray()) {
            return false;
        }
        m_arrayStack.push_back({&m_document, m_isArray});
        m_arrayIteratorStack.push_back(m_document.Begin());
    } else {
        if (m_isArray) {
            const auto &member = *m_arrayIteratorStack.back();
            if (!member.IsArray()) {
                return false;
            }
            m_arrayStack.push_back({&member, m_isArray});
            m_arrayIteratorStack.push_back(member.Begin());
        } else {
            if (!currentObject().HasMember(key)) {
                return false;
            }
            const auto &member = currentObject()[key];
            if (!member.IsArray()) {
                return false;
            }
            m_arrayStack.push_back({&member, m_isArray});
            m_arrayIteratorStack.push_back(member.Begin());
        }
    }
    m_isArray = true;
    return true;
}

bool JsonDecoder::arrayHasElement() {
    // std::cout << "NEXT ARRAY ELEMENT" << std::endl;
    if (m_arrayStack.empty() || m_arrayIteratorStack.back() == m_arrayStack.back().ref->End()) {
        return false;
    }
    return true;
}

void JsonDecoder::arrayNext() {
    ++m_arrayIteratorStack.back();
}

void JsonDecoder::endArray() {
    // assert(!m_arrayStack.empty());
    // std::cout << "END ARRAY" << std::endl;
    m_isArray = m_arrayStack.back().isPartOfArray;
    m_arrayStack.pop_back();
    m_arrayIteratorStack.pop_back();
}

const rapidjson::Value &JsonDecoder::currentObject() {
    return m_objectStack.empty() ? m_document : *m_objectStack.back().ref;
}

} // namespace Rain