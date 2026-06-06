#pragma once
#include "blocksub.h"



//barrelDestructable
//a destructible barrel prop that can be placed as a level hazard.
//has 3 HP and maps health → damage stage for animated sprite selection.
//stage 0 = intact, 1 = medium, 2 = destroyed/broken.
//the blast system in LevelBase::applyBlast() can also destroy this.
class BarrelDestructable : public BlockSub {
private:
    const int maxHp;   //const — needed before blockHealth in initializer order
    int       hp;

public:
    BarrelDestructable(Biome* biome)
        : BlockSub(biome, true, 3)   //health=3 propagated to Block
        , maxHp(3)
        , hp(3)
    {}

    bool isSolid()  const override { return true;  }
    bool isBarrel() const override { return true;  }
    Block* clone()  const override { return new BarrelDestructable(*this); }

    int getDamageStage() const override {
        if (hp <= 0)              return 2; //destroyed
        if (hp <= maxHp / 3)     return 2; //heavy damage
        if (hp <= (2*maxHp) / 3) return 1; //medium damage
        return 0;                           //intact
    }

    //hit the barrel — called by applyBlast or melee/projectile collision
    void hit(int amount = 1) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    bool isDestroyed() const { return hp <= 0; }
};