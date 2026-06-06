#pragma once
#include "../Level and Environment System/Level.h"
#include "Projectile.h"

using namespace sf;

// travels in a parabolic arc under gravity.
class BallisticProjectile : public Projectile {
protected:
  float gravity;
  bool isArc;
  bool affectedByTiles;

public:
  BallisticProjectile(double posX, double posY, double angle, double velX,
                      double velY, float damage, bool isPlayerOwned,
                      bool isArc = true, bool affectedByTiles = true)
      : Projectile(posX, posY, angle, velX, velY, damage, isPlayerOwned),
        gravity(0.4f), isArc(isArc), affectedByTiles(affectedByTiles) {}

  virtual ~BallisticProjectile() {}

  void setAffectedByTiles(bool v) { affectedByTiles = v; }
  bool getAffectedByTiles() const { return affectedByTiles; }

  void update(Level *level, float dt) override {
    if (!level || !isActive)
      return;

    // 1. Gravity
    if (isArc) {
      velocityY += gravity;
      if (velocityY > 20.0)
        velocityY = 20.0;
    }

    // 2. Move
    posX += velocityX;
    posY += velocityY;

    // 3. World bounds
    const double worldW = level->getWorldPixelW();
    const double worldH = level->getWorldPixelH();
    if (posX < 0.0 || posX > worldW || posY < 0.0 || posY > worldH) {
      die();
      return;
    }

    // 4.tile collision
    if (affectedByTiles) {
      int col = (posX / level->getBlockSize());
      int row = (posY / level->getBlockSize());
      if (level->isSolidAt(col, row) && !level->isPlatformAt(col, row))
        onHit();
    }
  }

  virtual void onHit() override = 0;
  virtual void draw(RenderWindow &window) override = 0;
};