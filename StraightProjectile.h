#pragma once
#include "Projectile.h"
#include "../Level and Environment System/Level.h"
#include <cmath>

using namespace sf;



//travels in a straight line at a constant speed derived from its angle.
class StraightProjectile : public Projectile {
private:
    float speed;
    bool  hasHitEntity;
    bool  affectedByTiles;

public:
    StraightProjectile(double posX, double posY, double angle, float speed,
                       float damage, bool isPlayerOwned, bool affectedByTiles = true)
        : Projectile(posX, posY, angle,
                     cos(angle) * speed,
                     sin(angle) * speed,
                     damage, isPlayerOwned)
        , speed(speed)
        , hasHitEntity(false)
        , affectedByTiles(affectedByTiles)
    {}

    virtual ~StraightProjectile() {}

    float getSpeed() const { return speed; }

    //single-hit guard — ensures a bullet can only damage one entity
    bool canBeHitBy(Projectile *p) {
        if (hasHitEntity) return false;
        hasHitEntity = true;
        die();
        return true;
    }

    void onCollideWith(Entity* other, float dt) override {
        //StraightProjectile needs to enforce single-hit before dispatching
        if (hasHitEntity) return;
        other->resolveCollision(this, dt);
    }

    void update(Level* level, float dt) override {
        posX = posX + velocityX;
        posY = posY + velocityY;

        const double worldW = level->getWorldPixelW();
        const double worldH = level->getWorldPixelH();
        if (posX < 0.0 || posX > worldW || posY < 0.0 || posY > worldH) {
            die();
            return;
        }

        if (affectedByTiles) {
            int col = (posX / level->getBlockSize());
            int row = (posY / level->getBlockSize());
            if (level->isSolidAt(col, row) && !level->isPlatformAt(col, row)) onHit();
        }
    }

    virtual void onHit() override = 0;
    virtual void draw(RenderWindow& window) override = 0;
};