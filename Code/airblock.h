#include "blocksub.h"


class AirBlock : public BlockSub {
public:
    AirBlock(Biome* biome): BlockSub(biome, false) {}   //air is never "destructible"
 
    bool   isAir()   const override { 
        return true;
     }
    Block* clone() const override { return new AirBlock(*this); }
};