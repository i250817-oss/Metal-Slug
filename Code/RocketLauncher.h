#pragma once
#include "FireArms.h"
#include "../../Projectile System/Rocket.h"
#include "../../Manager/SoundManager.h"

class RocketLauncher : public FireArms {
public:
    RocketLauncher() : FireArms(5, 0, 0.5f, 20, "Rocket Launcher") {}
    ~RocketLauncher() {}

    Projectile* fire(double posX, double posY, double angle, bool isPlayerOwned) override {
        if (!canFire() || bullets <= 0) return nullptr;
        fireTimer = 1.0f / fireRate;
        bullets--;
        SoundManager::playSound("sound/rocket_launcher.ogg");
        return new Rocket(posX, posY, angle, isPlayerOwned, 10, 5, true);
    }

    void reload() override { bullets = 20; }
};