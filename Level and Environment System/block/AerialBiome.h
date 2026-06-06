#pragma once
#include  "biomeSub.h"


class AerialBiome : public BiomeSub {
public:
    AerialBiome() : BiomeSub(0) {}
    bool isAerial() const override {
         return true;
         }
};