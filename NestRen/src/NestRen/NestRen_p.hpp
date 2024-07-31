#pragma once

#include "RendererI.hpp"
#include "Encoder/RendererCommand.hpp"

namespace NestRen {

struct Context {
public:
    void rendererExecuteCommands(const RendererCommandType *command);
private:
    RendererI* m_renderer;
    Frame *m_submit;
};

}