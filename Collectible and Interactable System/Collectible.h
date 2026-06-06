#pragma once
#include "../Entity Root/Entity.h"
using namespace sf;
// forward declarations
class Level;
class PlayerSoldier;
class Vehicle;

class Collectible : public Entity {
protected:
  bool isVanished;
  float timeToRemain;
  float width;
  float height;
  double velocityY;

  int animFrame;
  float animTimer;
  float animDelay;

public:
  Collectible(double posX, double posY, float timeToRemain, float w = 32,
              float h = 32)
      : Entity(posX, posY), isVanished(false), timeToRemain(timeToRemain),
        width(w), height(h), velocityY(0.0), animFrame(0), animTimer(0),
        animDelay(0.15f) {}

  virtual ~Collectible() {}

  // entity interface
  float getWidth() const override { return width; }
  float getHeight() const override { return height; }

  // entityManager sweep — a vanished collectible is removed automatically
  bool getIsAlive() const override { return !isVanished; }

  bool isHostileTarget() const override { return false; }
  int getFaction() const override { return FACTION_NEUTRAL; }

  void setIsVanished(bool v) { isVanished = v; }
  bool getIsVanished() const { return isVanished; }
  float getTimeToRemain() const { return timeToRemain; }

  // called by EntityManager when the active player overlaps this collectible
  virtual void onCollect(PlayerSoldier *player) = 0;

  void die() { isVanished = true; }

  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }

  void resolveCollision(PlayerSoldier *p, float dt) override { onCollect(p); }

  void resolveCollision(Vehicle *v, float dt) override;

  void update(Level *level, float dt) override;

  void draw(RenderWindow &window) override = 0;
};