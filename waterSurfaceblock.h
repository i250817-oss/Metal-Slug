#include "blocksub.h"


class WaterSurfaceBlock : public BlockSub {
public:
    WaterSurfaceBlock(Biome* biome): BlockSub(biome, false) {}   //water cannot be destroyed
 
    bool   isWater()     const override { 
        return true;
      }
    bool   isDeepWater() const override { 
        return false;
     } //surface 
    Block* clone() const override { return new WaterSurfaceBlock(*this); }
};