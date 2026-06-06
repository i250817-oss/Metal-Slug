#include "Vehicle.h"
#include "../Character System/PlayerSoldier.h"
#include "../Level and Environment System/Level.h"
#include "../Projectile System/Projectile.h"
#include "../Manager/SoundManager.h"

float Vehicle::getEffectiveFireRateMultiplier() const {
    if (getIsOccupied() && getDriver()) return getDriver()->getVehicleFireRateMultiplier();
    return 1.0f;
}

void Vehicle::takeDamage(int val) {
    DamagableEntity::takeDamage(val);
}

void Vehicle::die() {
    setIsAlive(false);
    SoundManager::playSound("sound/heavy_explosion.ogg");
    if (getDriver()) {
        getDriver()->setCurrentVehicle(nullptr);
        getDriver()->setIsOccupied(false);
        setDriver(nullptr);
    }
}

void Vehicle::update(Level *level, float dt) {
    if (!isAlive) return;
    if (fireCooldown > 0)
        fireCooldown -= (dt) * getEffectiveFireRateMultiplier();
    applyPhysics();
    tileCollisionImpl(level);

    if (getIsOccupied() && getDriver()) {
        getDriver()->setPosX(posX);
        getDriver()->setPosY(posY);
        getDriver()->setIsFacingLeft(isFacingLeft);
    }
}

void Vehicle::draw(RenderWindow& window) {
    if (!isAlive) return;
    vehicleSprite.setPosition(posX, posY);
    vehicleSprite.setScale(isFacingLeft ? -renderScaleX : renderScaleX, renderScaleY);
    window.draw(vehicleSprite);
}

void Vehicle::onCollideWith(Entity *other, float dt) {
    DamagableEntity::onCollideWith(other, dt);
}

void Vehicle::resolveCollision(PlayerSoldier* p, float dt) {
    if (!getIsOccupied() && Keyboard::isKeyPressed(Keyboard::T) && !p->getTWasPressed()) {
        setDriver(p);
        p->setCurrentVehicle(this);
        p->setIsOccupied(true);
        p->setTWasPressed(true); // prevent same-frame exit
    }
}

void Vehicle::resolveCollision(Projectile* p, float dt) {
    if (!p || !p->getIsAlive()) return;
    DamagableEntity::resolveCollision(p, dt);
}
