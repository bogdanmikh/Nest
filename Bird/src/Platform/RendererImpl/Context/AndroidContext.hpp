//
// Created by Bogdan on 30.10.2024.
//

#pragma once

#include "Bird/GraphicsContext.hpp"

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdint.h>
#include <android/native_window.h>
#include <android/log.h>

namespace Bird {

class AndroidContext : public GraphicsContext {
public:
    AndroidContext();
    ~AndroidContext() override;
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
};

} // namespace Bird
