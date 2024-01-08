#include <cstdint>

enum class VoxelType {
    NOTHING = 0,
    GROUND = 1,
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
