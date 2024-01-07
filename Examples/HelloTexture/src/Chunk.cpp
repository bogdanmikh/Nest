#include "Chunk.hpp"

Chunk::Chunk(uint32_t wight, uint32_t height, uint32_t depth)
  : mWight(wight), mHeight(height), mDepth(depth), mSize(mWight * mHeight * mDepth) {
    chunk = new unsigned char[mSize];
    for (int y = 0; y < mHeight; y++) {
        for (int z = 0; z < mDepth; ++z) {
            for (int x = 0; x < mWight; ++x) {
                set(x, y, z, y <= 5);
            }
        }
    }
}

Chunk::~Chunk() {
    delete[] chunk;
}
