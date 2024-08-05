#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Nest {

class Model3D {
public:
    Model3D() = default;
    Model3D(const char *path);
    void create(const char* path);
private:

};

}