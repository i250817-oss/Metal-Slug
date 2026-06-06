#pragma once
#include "StraightProjectile.h"

using namespace sf;



//continuous fire stream. 5 blocks range. 2 HP/sec. Instantly kills mummies.
class FlameStream : public StraightProjectile {
private:
  float streamLength;
  float lifeTime;
  float timer;

  //animation
  static Texture texture;
  static bool textureLoaded;
  Sprite sprite;
  IntRect frames[8];
  int numFrames;
  int currentFrame;
  float animTimer;
  float animDelay;

public:
  FlameStream(double posX, double posY, double angle, bool isPlayerOwned, bool affectedByTiles = true)
      : StraightProjectile(posX, posY, angle, 10, 0, isPlayerOwned, affectedByTiles),
        streamLength(10 * 64), lifeTime(0.8f), timer(lifeTime),
        numFrames(8), currentFrame(0), animTimer(0), animDelay(4.0f / 60.0f) {
    entityType = ENTITY_PROJECTILE;

    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/flame.png")) {
        textureLoaded = true;
      }
    }
    sprite.setTexture(texture);
    sprite.setScale(3, 3);

    //flame frames
    frames[0] = IntRect(79, 607, 32, 16);
    frames[1] = IntRect(71, 582, 40, 20);
    frames[2] = IntRect(63, 549, 48, 28);
    frames[3] = IntRect(47, 512, 64, 32);
    frames[4] = IntRect(31, 475, 80, 32);
    frames[5] = IntRect(15, 442, 96, 28);
    frames[6] = IntRect(23, 411, 88, 26);
    frames[7] = IntRect(31, 376, 80, 30);

    sprite.setTextureRect(frames[0]);

    //pivot from the base of the flame
    sprite.setOrigin(0, frames[0].height / 2.0f);
    sprite.setRotation((angle * 180.0 / M_PI));
  }

  ~FlameStream() {}

  float getStreamLength() const { return streamLength; }
  float getDamageValue() const override { return 0; }
  float getDamagePerSecond() const override { return 2; }

  float getWidth() const override { return 96 * 3; }
  float getHeight() const override { return 32 * 3; }

  void onHit() override {}

  void update(Level *level, float dt) override {
    (void)level;
    timer -= dt;
    if (timer <= 0)
      die();

    animTimer += dt;
    if (animTimer >= animDelay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % numFrames;
      sprite.setTextureRect(frames[currentFrame]);
      sprite.setOrigin(0, frames[currentFrame].height / 2.0f);
    }
  }

  void draw(RenderWindow& window) override {
    if (!isActive)
      return;

    sprite.setPosition(posX, posY);
    window.draw(sprite);
  }
};

