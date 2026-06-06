#include "blocksub.h"


class SolidBlock : public BlockSub {
public:
    SolidBlock(Biome* biome, bool destructible = true) : BlockSub(biome, destructible) {}
 
    bool   isSolid() const override {
         return true;
         }
    Block* clone() const override { return new SolidBlock(*this); }
};