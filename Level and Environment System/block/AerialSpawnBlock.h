#include "spawnblock.h"


class AerialSpawnBlock : public SpawnBlock {
public:
    AerialSpawnBlock(Biome* biome) : SpawnBlock(biome) {}
    bool   isAerialSpawn() const override { 
        return true;
     }
    Block* clone() const override { return new AerialSpawnBlock(*this); }
};

 