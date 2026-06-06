#include "spawnblock.h"


class PowSpawnBlock : public SpawnBlock {
public:
    PowSpawnBlock(Biome* biome) : SpawnBlock(biome) {}
    bool   isPowSpawn() const override { return true; }
    Block* clone() const override { return new PowSpawnBlock(*this); }
};
 