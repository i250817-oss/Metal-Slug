#pragma once
#include "../Entity Root/DamagableEntity.h"
#include <cmath>

using namespace sf;

static const float FUSION_ATTACK_RANGE = 600; 
static const float FUSION_FIRE_COOLDOWN_BASE = 0.25f; 

class EntityManager; 

// AI support unit.
class FusionCompanion : public DamagableEntity {
private:
  int survivorCount;
  bool active;
  float cooldownTimer;
  float fireTimer;

  bool hasTarget;
  double targetEnemyX, targetEnemyY;

  Texture fusionTexture;
  Sprite fusionSprite;

  inline double dist2(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  }

public:
  FusionCompanion(PlayerSoldier **players, int playerCount, bool isActive = false)
      : DamagableEntity(0, 0, 100), active(isActive), 
        cooldownTimer(0), fireTimer(0), hasTarget(false) {
    if (fusionTexture.loadFromFile("Sprites/Character/Companion.png")) {
      fusionSprite.setTexture(fusionTexture);
      fusionSprite.setScale(3, 3);
    }
  }

  virtual ~FusionCompanion() {}

  bool isActive() const { return active; }

  void tryActivate(double px, double py) {
    if (!active && cooldownTimer <= 0) {
      active = true;
      posX = px;
      posY = py;
    }
  }

  void update(Level *level, float dt) override {
    if (!active) {
      if (cooldownTimer > 0) cooldownTimer -= dt;
      return;
    }
    if (fireTimer > 0) fireTimer -= dt;
    applyPhysics();
    tileCollisionImpl(level);
  }

  void draw(RenderWindow &window) override {
    if (!active) return;
    fusionSprite.setPosition(posX, posY);
    window.draw(fusionSprite);
  }

  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }

  void followPlayer(double px, double py) {
    double dx = px - posX;
    double dy = py - posY;
    double d2 = dx * dx + dy * dy;
    if (d2 > 100 * 100) {
      velocityX = dx * 0.1;
      velocityY = dy * 0.1;
      isFacingLeft = (dx < 0.0);
    } else {
      velocityX = 0;
      velocityY = 0;
    }
  }

  void scanForTarget(Entity **entities, int count) {
    hasTarget = false;
    double bestDist2 = FUSION_ATTACK_RANGE * FUSION_ATTACK_RANGE;
    for (int i = 0; i < count; i++) {
      Entity *e = entities[i];
      if (!e || !e->getIsAlive() || !e->isEnemy()) continue;
      double d2 = dist2(posX, posY, e->getPosX(), e->getPosY());
      if (d2 < bestDist2) {
        bestDist2 = d2;
        targetEnemyX = e->getPosX();
        targetEnemyY = e->getPosY();
        hasTarget = true;
      }
    }
  }

  void attackNearestEnemy();

  float getWidth() const override { return 32 * 3; }
  float getHeight() const override { return 32 * 3; }
  void die() override { active = false; cooldownTimer = 30.0f; }
};