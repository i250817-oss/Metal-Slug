#pragma once
#include "Enemy.h"



//enemyBase
//intermediate class — mirrors BlockSub pattern.
//provides safe false-defaults for all pure virtuals so concrete enemies
//only override the ones that apply to them.
class EnemyBase : public Enemy {
protected:
    EnemyBase(float x, float y, const char* name) : Enemy(x, y, name) {}

public:
    virtual ~EnemyBase() {}

    //safe defaults — concrete subclass overrides only what it needs
    bool canFly()       const override { return false; }
    bool isUnderwater() const override { return false; }
    bool isVehicle()    const override { return false; }
};