#pragma once

#include <Nest.hpp>
#include "Panel.hpp"

class ContentBrowser final: public Layer {
public:
    ContentBrowser();
    ~ContentBrowser();
    void start() override;
    void update(double deltaTime) override;
    void detach() override;
    Panel panel;
};

