#include "blocksub.h"


class MountainPeakBlock : public BlockSub {
public:
    MountainPeakBlock(Biome* biome, bool destructible = true): BlockSub(biome, destructible) {}
 
    bool isSolid()const override { return true; }
    bool isMountainPeak() const override  { return true; } //extra identity for aerial logic
    Block* clone() const override { return new MountainPeakBlock(*this); }
};