#pragma once

#include "ContentBrowser.hpp"
#include <Nest.hpp>

class ContentBrowserLevel final : public Nest::Layer {
public:
    ContentBrowserLevel();
    ~ContentBrowserLevel() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override {}

private:
    ContentBrowser panel;
};
