#include "spawnblock.h"


class UndeadSpawnBlock : public SpawnBlock {
public:
    UndeadSpawnBlock(Biome* biome) : SpawnBlock(biome) {}
    bool   isUndeadSpawn() const override { return true; }
    Block* clone() const override { return new UndeadSpawnBlock(*this); }
};
 