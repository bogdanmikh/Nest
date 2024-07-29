#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/TypeRegistry.hpp"

namespace Rain {

JsonEncoder::JsonEncoder(bool pretty)
    : m_pretty(pretty)
    , m_isArray(false)
    , m_document()
    , m_objectStack()
    , m_arrayStack()
    , m_allocator(&m_document.GetAllocator()) {}

void JsonEncoder::encode(std::ostream &stream, void *data, Rain::TypeInfo info) {
    info.encoderFunc(nullptr, this, info, data);
    if (m_document.HasParseError()) {
        // Parse error
    }
    rapidjson::OStreamWrapper streamWrapper(stream);
    if (m_pretty) {
        rapidjson::PrettyWriter prettyWriter(streamWrapper);
        m_document.Accept(prettyWriter);
    } else {
        rapidjson::Writer writer(streamWrapper);
        m_document.Accept(writer);
    }
    m_objectStack.clear();
    m_arrayStack.clear();
    m_isArray = false;
    if (m_document.IsArray()) {
        m_document.Clear();
    } else {
        m_document.RemoveAllMembers();
    }
}

void JsonEncoder::beginObject(const char *key) {
    // if (key) {
    //     std::cout << "BEGIN OBJECT " << key << std::endl;
    // } else {
    //     std::cout << "BEGIN OBJECT" << std::endl;
    // }
    rapidjson::Value *added;
    if (m_objectStack.empty()) {
        m_document.SetObject();
        added = &m_document;
    } else {
        rapidjson::Value obj(rapidjson::kObjectType);
        addValue(key, obj);
        if (m_isArray) {
            uint32_t size = m_arrayStack.back().ref->GetArray().Size();
            added = &m_arrayStack.back().ref->GetArray()[size - 1];
        } else {
            added = &currentObject().FindMember(key)->value;
        }
    }
    m_objectStack.push_back({added, m_isArray});
    m_isArray = false;
}

void JsonEncoder::endObject() {
    // std::cout << "END OBJECT" << std::endl;
    // assert(!m_objectStack.empty());
    m_isArray = m_objectStack.back().isPartOfArray;
    m_objectStack.pop_back();
}

void JsonEncoder::beginArray(const char *key) {
    // if (key) {
    //     std::cout << "BEGIN ARRAY " << key << std::endl;
    // } else {
    //     std::cout << "BEGIN ARRAY" << std::endl;
    // }
    if (m_objectStack.empty()) {
        m_document.SetArray();
        m_arrayStack.push_back({&m_document, m_isArray});
    } else {
        rapidjson::Value arr(rapidjson::kArrayType);
        rapidjson::Value *added;
        if (m_isArray) {
            m_arrayStack.back().ref->PushBack(arr, *m_allocator);
            uint32_t size = m_arrayStack.back().ref->GetArray().Size();
            added = &m_arrayStack.back().ref->GetArray()[size - 1];
        } else {
            currentObject().AddMember(rapidjson::StringRef(key), arr, *m_allocator);
            added = &currentObject().FindMember(key)->value;
        }
        m_arrayStack.push_back({added, m_isArray});
    }
    m_isArray = true;
}

void JsonEncoder::endArray() {
    // std::cout << "END ARRAY" << std::endl;
    // assert(!m_arrayStack.empty());
    m_isArray = m_arrayStack.back().isPartOfArray;
    m_arrayStack.pop_back();
}

rapidjson::Value &JsonEncoder::currentObject() {
    return m_objectStack.empty() ? m_document : *m_objectStack.back().ref;
}

void JsonEncoder::encode(const char *key, const int &data) {
    rapidjson::Value value;
    value = data;
    addValue(key, value);
}

void JsonEncoder::encode(const char *key, const float &data) {
    rapidjson::Value value;
    value = data;
    addValue(key, value);
}

void JsonEncoder::encode(const char *key, const std::string &data) {
    rapidjson::Value value;
    value = rapidjson::StringRef(data.c_str());
    addValue(key, value);
}

void JsonEncoder::encode(const char *key, const char *&data) {
    rapidjson::Value value;
    value = rapidjson::StringRef(data);
    addValue(key, value);
}

void JsonEncoder::addValue(const char *key, rapidjson::Value &value) {
    if (m_isArray) {
        m_arrayStack.back().ref->PushBack(value, *m_allocator);
    } else {
        currentObject().AddMember(rapidjson::StringRef(key), value, *m_allocator);
    }
}

} // namespace Rain