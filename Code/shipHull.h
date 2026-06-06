#include "blocksub.h"


class ShipHullBlock : public BlockSub {
public:
    ShipHullBlock(Biome* biome, bool destructible = true): BlockSub(biome, destructible) {}
 
    bool   isSolid() const override { return true; }
    bool   isShipHull() const override { return true; }
    Block* clone() const override { return new ShipHullBlock(*this); }
};