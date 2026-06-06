#pragma once
#include "Biome.h"



//central altitude, flat terrain with small hills.
class PlainsBiome : public Biome {
public:
    PlainsBiome(int startX, int endX, int startY, int endY);
    ~PlainsBiome();
    void generate() override;
};