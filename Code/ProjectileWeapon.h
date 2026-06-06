#pragma once
#include "Weapon.h"

class ProjectileWeapon : public Weapon {
protected:
    float fireRate; 
    float fireTimer;

public:
    ProjectileWeapon(float dmg, float reloadTime, float fireRate, const char* name)
        : Weapon(dmg, reloadTime, name), fireRate(fireRate), fireTimer(0) {}

    virtual ~ProjectileWeapon() {}

    void tickCooldown(float dt) override {
        if (fireTimer > 0) fireTimer -= dt;
    }

    void resetFireTimer() override { fireTimer = 0; }

    bool canFire() const override { return fireTimer <= 0; }

    virtual Projectile* fire(double posX, double posY, double angle, bool isPlayerOwned) override = 0;
    virtual void reload() override = 0;
};