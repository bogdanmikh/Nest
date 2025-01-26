//
// Created by Bogdan on 23.11.2024.
//

#pragma once

#include <array>
#include <filesystem>
#include <optional>

struct ShaderEnvelope {
private:
    struct ShaderData {
        std::optional<std::string> code;
    };

public:
    enum ShaderTypes { VERTEX = 0, GEOMETRY, FRAGMENT, NONE = 3 };

    struct ShadersData {
        std::array<ShaderData, 3> shadersData;
    };
    static ShadersData openEnvelope(const std::filesystem::path &path);
};