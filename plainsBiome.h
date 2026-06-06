#pragma once
#include "biomeSub.h"


class PlainsBiome : public BiomeSub {
public:
    PlainsBiome() : BiomeSub(1) {}
    bool isPlains() const override { return true; }
};