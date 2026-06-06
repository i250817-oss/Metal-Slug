#include "blocksub.h"


class SeafloorBlock : public BlockSub {
public:
    SeafloorBlock(Biome* biome): BlockSub(biome, false) {}   //seafloor is structural; cannot be dug out
 
    bool   isSolid() const override { return true; }
    bool   isSeafloor() const override { return true; }
    Block* clone() const override { return new SeafloorBlock(*this); }
};