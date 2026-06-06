#pragma once
#include "Biome.h"



//lowest altitude. Fixed sea level. Vast enough for ships, deep enough for underwater combat.
class AquaticBiome : public Biome {
private:
    int seaLevel;

public:
    AquaticBiome(int startX, int endX, int startY, int endY, int seaLevel);
    ~AquaticBiome();
    int getSeaLevel() const;
    void generate() override;
};