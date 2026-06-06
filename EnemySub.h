#pragma once
#include "EnemyBase.h"



//enemy submarine — aquatic biome, underwater vehicle
//covers both "enemy sub" (L1/L2) and "aquatic sub" (L3)
class EnemySub : public EnemyBase {
public:
    EnemySub(float x, float y) : EnemyBase(x, y, "enemy_sub") {}
    bool   isUnderwater() const override { return true;  }
    bool   isVehicle()    const override { return true;  }
    Enemy* clone()        const override { return new EnemySub(*this); }
};