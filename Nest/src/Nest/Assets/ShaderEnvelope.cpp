//
// Created by Bogdan on 23.11.2024.
//

#include "Nest/Assets/ShaderEnvelope.hpp"

#include "pugixml.hpp"

#include <Foundation/Logger.hpp>

ShaderEnvelope::ShadersData ShaderEnvelope::openEnvelope(const std::filesystem::path &path) {
    ShadersData result;
    pugi::xml_document doc;

    if (!doc.load_file(path.string().c_str())) {
        LOG_ERROR("FILE NOT LOADED");
        return {};
    }

    pugi::xml_node vertexNode = doc.child("root").child("shaders").child("vertex");

    if (vertexNode) {
        const char *vertexCode = vertexNode.child_value();
        auto &val = result.shadersData[ShaderTypes::VERTEX].code;
        val.emplace();
        val.value() = vertexCode;
    }

    pugi::xml_node geometryNode = doc.child("root").child("shaders").child("geometry");

    if (geometryNode) {
        const char *geometryCode = geometryNode.child_value();
        auto &val = result.shadersData[ShaderTypes::GEOMETRY].code;
        val.emplace();
        val.value() = geometryCode;
    }

    pugi::xml_node fragmentNode = doc.child("root").child("shaders").child("fragment");

    if (fragmentNode) {
        const char *fragmentCode = fragmentNode.child_value();
        auto &val = result.shadersData[ShaderTypes::FRAGMENT].code;
        val.emplace();
        val.value() = fragmentCode;
    }
    return result;
}