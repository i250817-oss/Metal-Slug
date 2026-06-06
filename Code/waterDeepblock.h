#include "blocksub.h"


class WaterDeepBlock : public BlockSub {
public:
    WaterDeepBlock(Biome* biome): BlockSub(biome, false) {}
 
    bool   isWater()     const override {
         return true;
         }
    bool   isDeepWater() const override {
         return true; 
        } //deep 
    Block* clone() const override { return new WaterDeepBlock(*this); }
};