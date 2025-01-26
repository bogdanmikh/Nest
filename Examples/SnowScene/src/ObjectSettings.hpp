//
// Created by Bogdan on 21.10.2024.
//

#pragma once

#include "Nest/Base/Base.hpp"
#include <Rain/Codable.hpp>
#include <Rain/Rain.hpp>
#include <glm/glm.hpp>

struct ObjectSettings : public Rain::Codable {
    std::string name;
    Nest::Vec3 degrees = {0, 0, 0};
    Nest::Vec3 position = {0, 0, 0};
    Nest::Vec3 scale = {1, 1, 1};

    RAIN_FIELDS_BEGIN(ObjectSettings)
    RAIN_FIELD(name)
    RAIN_FIELD(degrees)
    RAIN_FIELD(position)
    RAIN_FIELD(scale)
    RAIN_FIELDS_END
};

struct ObjectsSettings : public Rain::Codable {
    std::vector<ObjectSettings> settings;

    RAIN_FIELDS_BEGIN(ObjectsSettings)
    RAIN_FIELD(settings)
    RAIN_FIELDS_END
};
