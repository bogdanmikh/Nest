#include <cstdint>

enum class VoxelType {
    NOTHING = 0,
    GROUND = 1,
    GRASS = 2,
    TREE = 3,
    BOARDS = 4,
    STONE = 5,
    STONE_BRICKS = 6,
    SAND_STONE = 7,
    SAND = 8,
    COUNT = 0
};

struct Voxel {
    VoxelType type;

    Voxel()
            : type(VoxelType::NOTHING) {}

    explicit Voxel(VoxelType type)
            : type(type) {}

    inline bool isAir() const {
        return type == VoxelType::NOTHING;
    }
};
