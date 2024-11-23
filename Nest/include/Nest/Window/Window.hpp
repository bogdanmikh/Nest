#pragma once

#include <glm/glm.hpp>
#include "Nest/Base/Base.hpp"

namespace Nest {

class Window {
public:
    virtual ~Window() = default;
    virtual void init(
        const char *name, uint32_t resolutionX = 1, uint32_t resolutionY = 1, bool fullScreen = true
    ) = 0;
    virtual void init(const char *name, bool fullScreen = true) = 0;
    virtual bool shouldClose() = 0;
    virtual void setShouldClose() = 0;
    virtual void swapBuffers() = 0;
    virtual Size getSize() = 0;
    virtual double getTime() = 0;
    virtual void *getNativeHandle() = 0;
};

} // namespace Nest