#pragma once
#include "EnemyBase.h"



//zombie — aquatic biome only
class Zombie : public EnemyBase {
public:
    Zombie(float x, float y) : EnemyBase(x, y, "zombie") {}
    Enemy* clone() const override { return new Zombie(*this); }
};