#pragma once
#include "block.h"



class BlockSub : public Block {
protected:
    BlockSub(Biome* biome, bool destructible, int health = 1)
        : Block(biome, destructible, health) {}
 
public:
    virtual ~BlockSub() {}
 
    //Define every pure virtual from Block with safe defaults 
    bool isAir()            const override { return false; }
    bool isSolid()          const override { return false; }
    bool isWater()          const override { return false; }
    bool isDeepWater()      const override { return false; }
    bool isSpawnMarker()    const override { return false; }
    bool isIndestructible() const override { return false; }
    bool isMountainPeak()   const override { return false; }
    bool isSeafloor()       const override { return false; }
    bool isShipHull()       const override { return false; }
    bool isPlatform()       const override { return false; }
    bool isBarrel()         const override { return false; }
};