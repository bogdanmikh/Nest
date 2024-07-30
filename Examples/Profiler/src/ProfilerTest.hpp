#pragma once

#include <Nest.hpp>

class ProfilerTest final : public Layer {
public:
    void start() override;
    void detach() override;
    void update(double deltaTime) override;
};
