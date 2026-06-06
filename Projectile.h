#pragma once
#include "../Entity Root/Entity.h"

using namespace sf;

// abstract base for all fired/thrown projectiles.
class Projectile : public Entity {
protected:
  double velocityX;
  double velocityY;
  float damage;
  double angle;
  bool isActive;
  bool isPlayerOwned;

public:
  Projectile(double posX, double posY, double angle, double velX, double velY,
             float damage, bool isPlayerOwned)
      : Entity(posX, posY), velocityX(velX), velocityY(velY), damage(damage),
        angle(angle), isActive(true), isPlayerOwned(isPlayerOwned) {}

  virtual ~Projectile() {}

  double getVelocityX() const { return velocityX; }
  double getVelocityY() const { return velocityY; }
  float getDamage() const { return damage; }
  double getAngle() const { return angle; }
  bool getIsActive() const { return isActive; }
  void setIsActive(bool v) { isActive = v; }

  virtual float getWidth() const override { return 6; }
  virtual float getHeight() const override { return 3; }

  // projectile Interface Overrides
  bool isHostileTarget() const override { return false; }
  float getDamageValue() const override { return damage; }
  bool getIsPlayerOwned() const override { return isPlayerOwned; }
  int getFaction() const override {
    return isPlayerOwned ? FACTION_PLAYER : FACTION_ENEMY;
  }

  // double Dispatch Collision Response
  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }

  virtual void onHit() = 0;
  void die() { isActive = false; }
  bool getIsAlive() const override { return isActive; }
  virtual void update(Level *level, float dt) override = 0;
  virtual void draw(RenderWindow &window) override = 0;
};
