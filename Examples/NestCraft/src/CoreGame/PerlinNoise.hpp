//
// Created by Bogdan
//

#pragma once

class PerlinNoise {
public:
    static void
    generate2DCustom(int seedValue, int octaves, float bias, float *values, int width, int height);
    static void
    generate2DGlm(int seedValue, int octaves, float bias, float *values, int width, int height);
};
