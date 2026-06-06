#pragma once
#include "BallisticProjectile.h"
#include "../Manager/SoundManager.h"

using namespace sf;

// arcing explosive. Explodes on contact with enemy or terrain.
class Grenade : public BallisticProjectile {
private:
  static Texture texture;
  static bool textureLoaded;
  static Texture blastTexture;
  static bool blastTextureLoaded;

  Sprite sprite;
  Sprite blastSprite;
  IntRect frames[16];
  IntRect blastFrames[15];

  bool isExploding;
  int explosionTimer;
  float blastRadius;
  int currentFrame;
  float animTimer;
  float animDelay;

public:
  Grenade(double posX, double posY, double angle, float speed, float damage,
          bool isPlayerOwned)
      : BallisticProjectile(posX, posY, angle, cos(angle) * speed,
                            sin(angle) * speed, damage, isPlayerOwned, true,
                            true),
        isExploding(false), explosionTimer(0), blastRadius(3 * 64),
        currentFrame(0), animTimer(0), animDelay(4.0f / 60.0f) {
    entityType = ENTITY_PROJECTILE;

    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/Marco Rossi 2.png")) {
        textureLoaded = true;
      }
    }
    if (!blastTextureLoaded) {
      if (blastTexture.loadFromFile(
              "Sprites/Enemies/Rebel Soldier (Bazooka).png")) {
        blastTextureLoaded = true;
      }
    }

    sprite.setTexture(texture);
    sprite.setScale(3, 3);
    blastSprite.setTexture(blastTexture);
    blastSprite.setScale(4, 4);

    // grenade frames
    frames[0] = IntRect(919, 8696, 11, 19);
    frames[1] = IntRect(935, 8695, 12, 20);
    frames[2] = IntRect(952, 8695, 13, 20);
    frames[3] = IntRect(970, 8696, 15, 18);
    frames[4] = IntRect(990, 8696, 17, 17);
    frames[5] = IntRect(1012, 8697, 18, 15);
    frames[6] = IntRect(1035, 8698, 19, 13);
    frames[7] = IntRect(1059, 8698, 19, 12);
    frames[8] = IntRect(1083, 8698, 18, 13);
    frames[9] = IntRect(1106, 8698, 19, 13);
    frames[10] = IntRect(1130, 8698, 18, 14);
    frames[11] = IntRect(1153, 8697, 17, 16);
    frames[12] = IntRect(1175, 8696, 17, 18);
    frames[13] = IntRect(1197, 8695, 15, 20);
    frames[14] = IntRect(1217, 8695, 13, 20);
    frames[15] = IntRect(1235, 8695, 11, 20);

    // blast frames from Rebel Soldier (Bazooka).png
    blastFrames[0] = IntRect(3, 771, 27, 27);
    blastFrames[1] = IntRect(33, 773, 23, 23);
    blastFrames[2] = IntRect(59, 774, 21, 21);
    blastFrames[3] = IntRect(83, 775, 23, 20);
    blastFrames[4] = IntRect(109, 772, 24, 24);
    blastFrames[5] = IntRect(136, 770, 28, 27);
    blastFrames[6] = IntRect(167, 768, 28, 29);
    blastFrames[7] = IntRect(198, 768, 32, 29);
    blastFrames[8] = IntRect(233, 767, 34, 30);
    blastFrames[9] = IntRect(270, 766, 34, 30);
    blastFrames[10] = IntRect(307, 766, 35, 29);
    blastFrames[11] = IntRect(345, 765, 34, 30);
    blastFrames[12] = IntRect(382, 765, 36, 30);
    blastFrames[13] = IntRect(421, 765, 30, 22);
    blastFrames[14] = IntRect(454, 765, 32, 25);

    sprite.setTextureRect(frames[0]);
    sprite.setOrigin(frames[0].width / 2.0f, frames[0].height / 2.0f);
  }

  bool isExplosiveProjectile() const override { return true; }

  float getWidth() const override { return isExploding ? blastRadius : 19 * 3; }
  float getHeight() const override {
    return isExploding ? blastRadius : 20 * 3;
  }

  void onHit() override { explode(); }

  void explode() {
    if (!isExploding) {
      isExploding = true;
      SoundManager::playSound("sound/grenade_explosion.ogg");
      explosionTimer = 8; // FIX: matches spec (0.13s)
      currentFrame = 0;
      animTimer = 0;
      animDelay = 2.0f / 60.0f; // fast explosion
      velocityX = 0;
      velocityY = 0;
      // center the explosion sprite
      posX -= (blastRadius / 2.0);
      posY -= (blastRadius / 2.0);
    }
  }

  void update(Level *level, float dt) override {
    if (isExploding) {
      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame++;
        if (currentFrame >= explosionTimer)
          die();
      }
    } else {
      BallisticProjectile::update(level, dt);
      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame = (currentFrame + 1) % 16;
        sprite.setTextureRect(frames[currentFrame]);
        sprite.setOrigin(frames[currentFrame].width / 2.0f,
                         frames[currentFrame].height / 2.0f);
      }
    }
  }

  void draw(RenderWindow &window) override {
    if (!isActive)
      return;

    if (isExploding) {
      if (currentFrame < 15) {
        blastSprite.setTextureRect(blastFrames[currentFrame % 15]);
        blastSprite.setPosition(posX, posY);
        window.draw(blastSprite);
      }
    } else {
      sprite.setPosition(posX, posY);
      window.draw(sprite);
    }
  }
};
