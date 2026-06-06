#pragma once
#include "Biome.h"



//high altitude, jagged peaks mountainous terrain.
class AerialBiome : public Biome {
public:
    AerialBiome(int startX, int endX, int startY, int endY);
    ~AerialBiome();
    void generate() override;
};