#pragma once
#include "Entity.h"

class DamagableEntity : virtual public Entity {
protected:
  int hp;
  int maxHp;
  bool isAlive;

  float velocityX;
  float velocityY;
  float acceleration;
  float max_speed;
  float gravity;
  float jump_strength;
  int jumpCount;
  int maxJumps;
  bool isFacingLeft;
  bool isOnGround;
  bool isFlying;

  int currentFrame;
  float animTimer;
  float animDelay;

  float damageAccumulator;
  float lastDamageSourceX;

public:
  DamagableEntity() : Entity(0,0), hp(0), maxHp(0), isAlive(true), velocityX(0), velocityY(0), acceleration(0.5f), max_speed(5), gravity(0.5f), jump_strength(15), jumpCount(0), maxJumps(1), isFacingLeft(false), isOnGround(false), isFlying(false), currentFrame(0), animTimer(0), animDelay(6), damageAccumulator(0), lastDamageSourceX(-1) {}
  DamagableEntity(double posX, double posY, int maxHp)
      : Entity(posX, posY), hp(maxHp), maxHp(maxHp), isAlive(true),
        velocityX(0), velocityY(0), acceleration(0.5f), max_speed(5),
        gravity(0.5f), jump_strength(15), jumpCount(0), maxJumps(1),
        isFacingLeft(false), isOnGround(false), isFlying(false), currentFrame(0), animTimer(0),
        animDelay(6), damageAccumulator(0), lastDamageSourceX(-1) {}

  virtual ~DamagableEntity() {}

  DamagableEntity *asTarget() override { return this; }

  void setHp(int v) { hp = v; }
  int getHp() const { return hp; }
  int getMaxHp() const { return maxHp; }
  void setIsAlive(bool v) { isAlive = v; }
  bool getIsAlive() const override { return isAlive; }

  float getVelocityX() const { return velocityX; }
  void setVelocityX(float vx) { velocityX = vx; }
  float getVelocityY() const { return velocityY; }
  void setVelocityY(float vy) { velocityY = vy; }
  bool getIsFacingLeft() const { return isFacingLeft; }
  void setIsFacingLeft(bool v) { isFacingLeft = v; }
  bool getIsOnGround() const { return isOnGround; }
  void setIsOnGround(bool v) { isOnGround = v; }
  bool getIsFlying() const { return isFlying; }
  void setIsFlying(bool v) { isFlying = v; }

  void applyPhysics() {
    if (isFlying) {
        velocityY *= 0.9f; // Air friction
        return;
    }
    velocityY += gravity;
    if (velocityY > 20)
      velocityY = 20;
  }

  virtual void moveRight() {
    velocityX += acceleration;
    if (velocityX > max_speed)
      velocityX = max_speed;
    isFacingLeft = false;
  }

  virtual void moveLeft() {
    velocityX -= acceleration;
    if (velocityX < -max_speed)
      velocityX = -max_speed;
    isFacingLeft = true;
  }

  virtual void stopX() { velocityX = 0; }

  virtual void jump() {
    if (jumpCount < maxJumps) {
      velocityY = -jump_strength;
      jumpCount++;
      isOnGround = false;
    }
  }

  void onGroundCollision(float groundY) {
    velocityY = 0;
    jumpCount = 0;
    isOnGround = true;
  }

  virtual float getWidth() const override = 0;
  virtual float getHeight() const override = 0;

  virtual void takeDamage(int val) {
    hp -= val;
    if (hp <= 0) {
      hp = 0;
      isAlive = false;
      die();
    }
  }

  float getLastDamageSourceX() const { return lastDamageSourceX; }

  void applyFractionalDamage(float amount) {
    if (amount <= 0 || !isAlive)
      return;
    damageAccumulator += amount;
    while (damageAccumulator >= 1 && isAlive) {
      takeDamage(1);
      damageAccumulator -= 1;
    }
  }

  void onCollideWith(Entity *other, float dt) override {
    if (!isHostileTarget())
      return;
    other->resolveCollision(this, dt);
  }

  void resolveCollision(Projectile *p, float dt) override;

  virtual void onWallCollision() {}

  virtual void die() = 0;

  virtual void update(Level *level, float dt) override = 0;
  virtual void draw(RenderWindow &window) override = 0;

protected:
  bool checkTileCollisionAt(float x, float y, Level *level,
                            bool ignorePlatforms = false);
  bool checkFloorCollisionAt(float x, float y, Level *level,
                             bool ignorePlatforms = false);
  void tileCollisionImpl(Level *level);
};

#include "../Level and Environment System/Level.h"
#include "../Projectile System/Projectile.h"

using namespace sf;

inline void DamagableEntity::resolveCollision(Projectile *p, float dt) {
  if (!p || !p->getIsAlive())
    return;

  // faction filtering is handled by EntityManager — if we reach here,
  // the projectile is guaranteed hostile to this entity.
  float dps = p->getDamagePerSecond();
  if (dps > 0) {
    lastDamageSourceX = -1;
    applyFractionalDamage(dps * dt);
    return;
  }

  float dmg = p->getDamageValue();
  if (dmg <= 0)
    return;

  lastDamageSourceX = p->isExplosiveProjectile() ? -1 : p->getPosX();
  takeDamage(dmg);
  p->onHit();
}

inline bool DamagableEntity::checkTileCollisionAt(float x, float y,
                                                  Level *level,
                                                  bool ignorePlatforms) {
  if (!level)
    return false;
  const int bSz = level->getBlockSize();
  const float epsilon = 0.1f;
  int left   = (int)((x + epsilon) / bSz);
  int right  = (int)((x + getWidth() - epsilon) / bSz);
  int top    = (int)((y + 0.1f) / bSz);
  int bottom = (int)((y + getHeight() - 4) / bSz);
  if (left > right) right = left;
  if (top  > bottom) bottom = top;

  for (int col = left; col <= right; col++) {
    for (int row = top; row <= bottom; row++) {
      if (level->isSolidAt(col, row)) {
        if (ignorePlatforms && level->isPlatformAt(col, row))
          continue;
        return true;
      }
    }
  }
  return false;
}

inline bool DamagableEntity::checkFloorCollisionAt(float x, float y,
                                                   Level *level,
                                                   bool ignorePlatforms) {
  if (!level)
    return false;
  const int bSz = level->getBlockSize();
  // Inset is 10% of entity width from each side — works for any entity size
  float w = getWidth();
  float feetInset = w * 0.10f;
  if (feetInset < 2) feetInset = 2; // minimum 2px so narrow entities still probe

  int left  = (int)((x + feetInset) / bSz);
  int right = (int)((x + w - feetInset) / bSz);
  if (left > right) right = left; // safety: never invert range
  int bottom = (int)((y + getHeight() + 1) / bSz);

  for (int col = left; col <= right; col++) {
    if (level->isSolidAt(col, bottom)) {
      if (ignorePlatforms && level->isPlatformAt(col, bottom))
        continue;
      return true;
    }
  }
  return false;
}

inline void DamagableEntity::tileCollisionImpl(Level *level) {
  if (isFlying) {
      posX += velocityX;
      posY += velocityY;
      return;
  }
  if (!level)
    return;
  float worldW = level->getWorldPixelW();
  float worldH = level->getWorldPixelH();
  float pW = getWidth();
  float pH = getHeight();

  // y movement
  float nextY = posY + velocityY;
  if (nextY + pH > worldH) {
    onGroundCollision(worldH);
    posY = worldH - pH;
  } else if (nextY < 0) {
    posY = 0;
    velocityY = 0;
  } else {
    bool ignorePlatforms = (velocityY < 0);
    bool hasCollision = false;
    if (velocityY >= 0) {
      hasCollision = checkFloorCollisionAt(posX, nextY, level, ignorePlatforms);
    } else {
      hasCollision = checkTileCollisionAt(posX, nextY, level, ignorePlatforms);
    }

    if (!hasCollision) {
      posY = nextY;
      isOnGround = false;
    } else {
      int bSz = level->getBlockSize();
      if (velocityY > 0) {
        int floorRow = ((nextY + pH) / bSz);
        posY = floorRow * bSz - pH - 0.01f;
        onGroundCollision(floorRow * bSz);
      } else {
        int topRow = ((nextY + 0.1f) / bSz);
        posY = (topRow + 1) * bSz + 0.01f;
        velocityY = 0;
      }
    }
  }

  // x movement
  float nextX = posX + velocityX;
  if (nextX < 0) {
    posX = 0;
    velocityX = 0;
    onWallCollision();
  } else if (nextX + pW > worldW) {
    posX = worldW - pW;
    velocityX = 0;
    onWallCollision();
  } else {
    if (!checkTileCollisionAt(nextX, posY, level, true)) {
      posX = nextX;
    } else {
      int bSz = level->getBlockSize();
      if (velocityX > 0) {
        int rightCol = ((nextX + pW - 0.1f) / bSz);
        posX = rightCol * bSz - pW - 0.01f;
        velocityX = 0;
      } else if (velocityX < 0) {
        int leftCol = ((nextX + 0.1f) / bSz);
        posX = (leftCol + 1) * bSz + 0.01f;
        velocityX = 0;
      }
      velocityX = 0;
      onWallCollision();
    }
  }
}