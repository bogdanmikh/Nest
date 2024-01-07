#pragma once

#include <cstdint>

class Chunk {
private:
    uint32_t mWight;
    uint32_t mHeight;
    uint32_t mDepth;
    uint32_t mSize;
public:
    unsigned char *chunk;
    Chunk(uint32_t wight, uint32_t height, uint32_t depth);
    ~Chunk();
    inline uint32_t getSize() const {
        return mSize;
    }
    inline uint32_t getW() const {
        return mWight;
    }
    inline uint32_t getH() const {
        return mHeight;
    }
    inline uint32_t getD() const {
        return mDepth;
    }

    void set(uint32_t x, uint32_t y, uint32_t z, uint8_t block) {
        chunk[(y * mDepth + z) * mWight + x] = block;
    }

    unsigned char get(int32_t x, int32_t y, int32_t z) {
        return chunk[(y * mDepth + z) * mWight + x];
    }
};