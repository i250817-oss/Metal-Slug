#include "blocksub.h"


class IndestructibleBlock : public BlockSub {
public:
    IndestructibleBlock(Biome* biome): BlockSub(biome, false) {}   
 
    bool   isSolid() const override { return true; }
    bool   isIndestructible() const override { return true; }
    Block* clone() const override { return new IndestructibleBlock(*this); }
};
 