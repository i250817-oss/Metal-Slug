#pragma once
#include "StraightProjectile.h"

using namespace sf;

// standard bullet. High speed, low damage.
class Bullet : public StraightProjectile {
private:
  static Texture texture;
  static bool textureLoaded;
  Sprite sprite;
  IntRect frames[4];
  int currentFrame;
  float animTimer;
  float animDelay;

public:
  Bullet(double posX, double posY, double angle, float speed, float damage,
         bool isPlayerOwned, bool affectedByTiles = true)
      : StraightProjectile(posX, posY, angle, speed, damage, isPlayerOwned,
                           affectedByTiles),
        currentFrame(0), animTimer(0), animDelay(5.0f / 60.0f) {
    entityType = ENTITY_PROJECTILE;

    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/Marco Rossi 2.png")) {
        textureLoaded = true;
      }
    }
    sprite.setTexture(texture);
    sprite.setScale(3, 3);

    // bullet frames (provided by user)
    frames[0] = IntRect(49, 6844, 5, 7);
    frames[1] = IntRect(34, 6842, 11, 12);
    frames[2] = IntRect(24, 6845, 6, 6);
    frames[3] = IntRect(10, 6843, 10, 10);

    sprite.setTextureRect(frames[0]);
    sprite.setOrigin(frames[0].width / 2.0f, frames[0].height / 2.0f);
    sprite.setRotation((angle * 180.0 / M_PI));
  }

  // default constructor for simple usage
  Bullet(double posX, double posY, double angle, bool isPlayerOwned)
      : Bullet(posX, posY, angle, 16, 1, isPlayerOwned) {}

  float getWidth() const override { return 11 * 3; }
  float getHeight() const override { return 12 * 3; }

  void onHit() override { die(); }

  void update(Level *level, float dt) override {
    StraightProjectile::update(level, dt);
    if (!isActive)
      return;

    animTimer += dt;
    if (animTimer >= animDelay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % 4;
      sprite.setTextureRect(frames[currentFrame]);
      sprite.setOrigin(frames[currentFrame].width / 2.0f,
                       frames[currentFrame].height / 2.0f);
    }
  }

  void draw(RenderWindow &window) override {
    if (!isActive)
      return;
    sprite.setPosition(posX, posY);
    window.draw(sprite);
  }
};

// initialize static members
