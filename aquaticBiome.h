#pragma once
#include "biomeSub.h"


class AquaticBiome : public BiomeSub {
public:
    AquaticBiome() : BiomeSub(2) {}
    bool isAquatic() const override { return true; }
};