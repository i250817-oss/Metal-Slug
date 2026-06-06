#include "blocksub.h"


class PlatformBlock : public BlockSub {
public:
    PlatformBlock(Biome* biome, bool destructible = true) : BlockSub(biome, destructible) {}
 
    bool   isSolid() const override { return true; }
    bool   isPlatform() const override { return true; }
    Block* clone() const override { return new PlatformBlock(*this); }
};