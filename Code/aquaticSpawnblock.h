#include "spawnblock.h"


class AquaticSpawnBlock : public SpawnBlock {
public:
    AquaticSpawnBlock(Biome* biome) : SpawnBlock(biome) {}
    bool   isAquaticSpawn() const override { return true; }
    Block* clone() const override { return new AquaticSpawnBlock(*this); }
};
 