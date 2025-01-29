//
// Created by Admin on 18.02.2022.
//

#pragma once

#include <string>

namespace Bird {

// Указатели на главные элементы платформы (окно, контекст для рендера и т. д.)
class PlatformData {
public:
    static PlatformData *get();
    void *layer;
    void *nativeWindowHandle;
    void *renderingContext;

    void *assetManager; // for android
};

} // namespace Bird