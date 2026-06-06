#include"spawnblock.h"


class BradleySpawnBlock : public SpawnBlock {
public:
    BradleySpawnBlock(Biome* biome) : SpawnBlock(biome) {}
    bool   isBradleySpawn() const override { return true; }
    Block* clone() const override { return new BradleySpawnBlock(*this); }
};