//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/GraphicsContext.hpp"

namespace Bird {

class GlesContext : public GraphicsContext {
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    void *context;
};

} // namespace Bird
