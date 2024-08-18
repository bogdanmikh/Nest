#pragma once

#include <Nest.hpp>
#include "ContentBrowser.hpp"

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
