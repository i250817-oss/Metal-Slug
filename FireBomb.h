#pragma once
#include "BallisticProjectile.h"
#include "../Manager/SoundManager.h"

using namespace sf;

class FireBomb : public BallisticProjectile {
private:
  bool isExploding;
  float poolDuration;
  float blastRadius;
  float groundX;
  float groundY;

  static Texture texture;
  static bool textureLoaded;
  static Texture grenadeTexture;
  static bool grenadeTextureLoaded;

  Sprite fireSprite;
  Sprite grenadeSprite;

  IntRect fireFrames[28];
  IntRect grenadeFrames[16];

  int currentFrame;
  float animTimer;
  float animDelay;

public:
  FireBomb(double posX, double posY, double angle, float speed,
           bool isPlayerOwned)
      : BallisticProjectile(posX, posY, angle, cos(angle) * speed,
                            sin(angle) * speed, 0, isPlayerOwned),
        isExploding(false), poolDuration(10), blastRadius(3 * 64),
        groundX(0), groundY(0),
        currentFrame(0), animTimer(0), animDelay(4.0f / 60.0f) {
    entityType = ENTITY_PROJECTILE;

    // fire pool texture
    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/fire.png")) {
        textureLoaded = true;
      }
    }

    // grenade projectile texture
    if (!grenadeTextureLoaded) {
      if (grenadeTexture.loadFromFile("Sprites/Marco Rossi 2.png")) {
        grenadeTextureLoaded = true;
      }
    }

    fireSprite.setTexture(texture);
    fireSprite.setScale(2, 2);
    grenadeSprite.setTexture(grenadeTexture);
    grenadeSprite.setScale(3, 3);

    // flame frames
    fireFrames[0] = IntRect(19, 10, 38, 50);
    fireFrames[1] = IntRect(76, 14, 40, 46);
    fireFrames[2] = IntRect(132, 9, 47, 51);
    fireFrames[3] = IntRect(192, 0, 53, 58);
    fireFrames[4] = IntRect(265, 7, 51, 67);
    fireFrames[5] = IntRect(330, 2, 37, 71);
    fireFrames[6] = IntRect(383, 4, 57, 71);
    fireFrames[7] = IntRect(454, 0, 51, 75);
    fireFrames[8] = IntRect(518, 4, 50, 75);
    fireFrames[9] = IntRect(38, 84, 43, 60);
    fireFrames[10] = IntRect(103, 76, 44, 69);
    fireFrames[11] = IntRect(159, 63, 41, 85);
    fireFrames[12] = IntRect(224, 71, 37, 73);
    fireFrames[13] = IntRect(276, 86, 75, 57);
    fireFrames[14] = IntRect(365, 96, 93, 47);
    fireFrames[15] = IntRect(459, 93, 125, 46);
    fireFrames[16] = IntRect(22, 154, 57, 65);
    fireFrames[17] = IntRect(92, 151, 48, 70);
    fireFrames[18] = IntRect(159, 156, 46, 65);
    fireFrames[19] = IntRect(221, 158, 64, 64);
    fireFrames[20] = IntRect(297, 152, 72, 68);
    fireFrames[21] = IntRect(382, 144, 42, 93);
    fireFrames[22] = IntRect(443, 142, 96, 85);
    fireFrames[23] = IntRect(1, 229, 115, 86);
    fireFrames[24] = IntRect(136, 225, 67, 90);
    fireFrames[25] = IntRect(220, 229, 96, 83);
    fireFrames[26] = IntRect(333, 245, 180, 67);
    fireFrames[27] = IntRect(558, 189, 80, 117);

    // grenade frames (matching Grenade.h)
    grenadeFrames[0] = IntRect(919, 8696, 11, 19);
    grenadeFrames[1] = IntRect(935, 8695, 12, 20);
    grenadeFrames[2] = IntRect(952, 8695, 13, 20);
    grenadeFrames[3] = IntRect(970, 8696, 15, 18);
    grenadeFrames[4] = IntRect(990, 8696, 17, 17);
    grenadeFrames[5] = IntRect(1012, 8697, 18, 15);
    grenadeFrames[6] = IntRect(1035, 8698, 19, 13);
    grenadeFrames[7] = IntRect(1059, 8698, 19, 12);
    grenadeFrames[8] = IntRect(1083, 8698, 18, 13);
    grenadeFrames[9] = IntRect(1106, 8698, 19, 13);
    grenadeFrames[10] = IntRect(1130, 8698, 18, 14);
    grenadeFrames[11] = IntRect(1153, 8697, 17, 16);
    grenadeFrames[12] = IntRect(1175, 8696, 17, 18);
    grenadeFrames[13] = IntRect(1197, 8695, 15, 20);
    grenadeFrames[14] = IntRect(1217, 8695, 13, 20);
    grenadeFrames[15] = IntRect(1235, 8695, 11, 20);

    fireSprite.setTextureRect(fireFrames[0]);
    grenadeSprite.setTextureRect(grenadeFrames[0]);
    grenadeSprite.setOrigin(grenadeFrames[0].width / 2.0f,
                            grenadeFrames[0].height / 2.0f);
  }

  ~FireBomb() {}

  bool isExplosiveProjectile() const override { return true; }
  float getDamageValue() const override { return 0; }
  float getDamagePerSecond() const override { return isExploding ? 2 : 0; }
  float getWidth() const override { return isExploding ? blastRadius : 19 * 3; }
  float getHeight() const override {
    return isExploding ? blastRadius : 20 * 3;
  }

  void onHit() override {
    if (!isExploding) {
      isExploding = true;
      SoundManager::playSound("sound/grenade_explosion.ogg");
      velocityX = 0;
      velocityY = 0;
      currentFrame = 0; // reset for fire pool animation
      groundX = posX;
      groundY = posY;
      posX -= blastRadius / 2.0;
      posY -= blastRadius / 2.0;
      animDelay = 10.0f / 60.0f; // Slower fire pool animation
    }
  }

  void update(Level *level, float dt) override {
    if (isExploding) {
      poolDuration -= dt;
      if (poolDuration <= 0)
        die();

      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame = (currentFrame + 1) % 28;
        fireSprite.setTextureRect(fireFrames[currentFrame]);
      }
    } else {
      BallisticProjectile::update(level, dt);

      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame = (currentFrame + 1) % 16;
        grenadeSprite.setTextureRect(grenadeFrames[currentFrame]);
        grenadeSprite.setOrigin(grenadeFrames[currentFrame].width / 2.0f,
                                grenadeFrames[currentFrame].height / 2.0f);
      }
    }
  }

  void draw(RenderWindow &window) override {
    if (!isActive)
      return;

    if (isExploding) {
      fireSprite.setOrigin(fireFrames[currentFrame].width / 2.0f,
                           fireFrames[currentFrame].height);
      fireSprite.setPosition(groundX, groundY);
      window.draw(fireSprite);
    } else {
      grenadeSprite.setPosition(posX, posY);
      window.draw(grenadeSprite);
    }
  }
};

// initialize static members
