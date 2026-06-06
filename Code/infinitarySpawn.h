#include "spawnblock.h"


class InfantrySpawnBlock : public SpawnBlock {
public:
    InfantrySpawnBlock(Biome* biome) : SpawnBlock(biome) {}
    bool   isInfantrySpawn() const override { return true; }
    Block* clone() const override { return new InfantrySpawnBlock(*this); }
};