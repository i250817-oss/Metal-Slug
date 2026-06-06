#pragma once
#include "../NonProjectileWeapon.h"



//knife
//melee weapon. 2 HP damage. Range: 2 tile-blocks (128 px). 0.5 s cooldown.
//attack() activates the hitbox; EntityManager::handleCollisions() applies damage.
class Knife : public NonProjectileWeapon {

    static const int KNIFE_RANGE    = 128;   //2 blocks * 64 px
    static const int KNIFE_DAMAGE   = 2;
    static const int KNIFE_COOLDOWN = 1;     //0.5 s — stored as float below

public:
    Knife()
        : NonProjectileWeapon(
            KNIFE_DAMAGE,
            KNIFE_RANGE,
            0.5f,               //0.5 s cooldown
            "Knife")
    {}

    virtual ~Knife() {}

    //attack
    //called by PlayerSoldier::shoot() when Space is pressed and the current
    //weapon is the Knife.  Activates the swing hitbox via NonProjectileWeapon.
    //entityManager::handleCollisions() reads getIsAttacking() each frame and
    //deals getDamage() to any overlapping DamagableEntity of the opposing team.
    void attack(double posX, double posY, bool facingRight) override
    {
        //base handles cooldown guard + sets isAttacking / attackTimer
        NonProjectileWeapon::attack(posX, posY, facingRight);
    }

    //canPierceShield — true when wielded by MarcoRossi (checked by subclass)
    bool canPierceShield() const { return false; }
};