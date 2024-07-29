#pragma once

#include <Nest.hpp>

class Profiler final : public Layer {
public:
    void start() override;
    void detach() override;
    void update(double deltaTime) override;
};
