#pragma once
#include "../../Projectile System/Bullet.h"
#include "../../Weapon System/FireArms/Pistol.h"
#include "../Infantry/InfantryEnemy.h"
#include <cstdlib>

using namespace sf;
using namespace std;

class AerialParatrooper : public InfantryEnemy {
private:
  InfantryEnemy *payload;

  // descent state
  bool isDescending;
  float descentSpeed;
  float driftX;
  float driftTimer;
  float driftChangeTime;

  // mid-air shots
  int airShotsRemaining;
  float airShotTimer;
  float airShotInterval;

  // landing animation
  bool isLanding;
  int landFrame;
  float landTimer;
  bool isActivated;

  // sprite assets
  Texture chuteTexture;
  Sprite chuteSprite;

  IntRect chuteFrames[7];
  int numChuteFrames;
  IntRect landFrames[8];
  int numLandFrames;

  // air pistol for downward shots
  Texture airGunTexture;

  // Death animation
  IntRect deathFrames[11];
  bool isDying;
  int deathFrame;
  float deathAnimTimer;

public:
  AerialParatrooper(double posX, double posY, InfantryEnemy *payloadEnemy)
      : Entity(posX, posY), DamagableEntity(posX, posY, payloadEnemy ? payloadEnemy->getMaxHp() : 2),
        InfantryEnemy(posX, posY, payloadEnemy ? payloadEnemy->getMaxHp() : 2),
        payload(payloadEnemy), isDescending(true), descentSpeed(1.8f),
        driftX(0), driftTimer(0), driftChangeTime(0), airShotsRemaining(2),
        airShotTimer(0), airShotInterval(3.5f) // first air shot after 3.5s
        ,
        isLanding(false), landFrame(0), landTimer(0), isActivated(false),
        numChuteFrames(7), numLandFrames(8) {
    setWeapon(new Pistol());

    isDying = false;
    deathFrame = 0;
    deathAnimTimer = 0.0f;

    deathFrames[0]  = IntRect(5, 10297, 26, 37);
    deathFrames[1]  = IntRect(36, 10297, 25, 37);
    deathFrames[2]  = IntRect(66, 10298, 24, 36);
    deathFrames[3]  = IntRect(95, 10299, 24, 35);
    deathFrames[4]  = IntRect(124, 10300, 27, 34);
    deathFrames[5]  = IntRect(156, 10301, 27, 33);
    deathFrames[6]  = IntRect(188, 10303, 27, 31);
    deathFrames[7]  = IntRect(220, 10307, 27, 27);
    deathFrames[8]  = IntRect(252, 10312, 26, 22);
    deathFrames[9]  = IntRect(283, 10316, 28, 18);
    deathFrames[10] = IntRect(316, 10316, 29, 18);

    if (chuteTexture.loadFromFile(
            "Sprites/Enemies/Rebel Soldier (Bazooka).png")) {
      chuteSprite.setTexture(chuteTexture);
      chuteSprite.setScale(3, 3);
    }

    // descending parachute (7 frames, y=861)
    chuteFrames[0] = IntRect(3, 861, 52, 43);
    chuteFrames[1] = IntRect(58, 861, 51, 43);
    chuteFrames[2] = IntRect(167, 861, 52, 43);
    chuteFrames[3] = IntRect(222, 861, 52, 43);
    chuteFrames[4] = IntRect(277, 861, 52, 43);
    chuteFrames[5] = IntRect(332, 861, 52, 43);
    chuteFrames[6] = IntRect(387, 861, 51, 43);

    // chute dismantling after landing (8 frames, y=953)
    landFrames[0] = IntRect(3, 953, 55, 43);
    landFrames[1] = IntRect(61, 955, 54, 44);
    landFrames[2] = IntRect(118, 959, 56, 43);
    landFrames[3] = IntRect(177, 963, 55, 47);
    landFrames[4] = IntRect(235, 968, 53, 44);
    landFrames[5] = IntRect(291, 975, 54, 40);
    landFrames[6] = IntRect(348, 986, 56, 34);
    landFrames[7] = IntRect(407, 995, 62, 26);

    animDelay = 8.0f / 60.0f;
    chuteSprite.setTextureRect(chuteFrames[0]);

    pickNewDrift();
  }

  ~AerialParatrooper() {
    delete payload;
    payload = nullptr;
  }

  float getWidth() const override {
    return payload ? payload->getWidth() : 52 * 3;
  }
  float getHeight() const override {
    return payload ? payload->getHeight() : 43 * 3;
  }

  bool getIsDescending() const { return isDescending; }

  bool isAerialKill() const override { return true; }

  bool getIsDying() const override { return isDying; }

  void takeDamage(int val) override {
    if (isDying) return;
    if (hp - val > 0) {
      InfantryEnemy::takeDamage(val);
    } else {
      hp = 0;
      isDying = true;
      deathFrame = 0;
      deathAnimTimer = 0.0f;
      velocityX = 0;
      velocityY = 0;
      playDeathSound();
    }
  }

  void update(Level *level, float dt) override {
    if (!isActivated) {
      if (distanceToPlayer() < 15) {
        isActivated = true;
      } else {
        return; // stay frozen until player is close
      }
    }

    if (isDying) {
      deathAnimTimer += dt;
      float frameDelay = 6.0f / 60.0f;
      if (deathAnimTimer >= frameDelay) {
        deathAnimTimer = 0;
        deathFrame++;
      }
      if (deathFrame >= 11) {
        isDying = false;
        InfantryEnemy::takeDamage(9999);
        return;
      }
      applyPhysics();
      tileCollisionImpl(level);
      enemySprite.setTexture(retreatTexture);
      enemySprite.setTextureRect(deathFrames[deathFrame]);
      return;
    }

    if (isDescending) {
      updateDrift(dt);
      updateAirShots(dt);

      velocityX = driftX;
      velocityY = descentSpeed;
      tileCollisionImpl(level);

      if (target)
        isFacingLeft = (target->getPosX() < posX);

      if (payload) {
        payload->setOverrideRect(IntRect(3, 830, 38, 38));
      }

      // animation logic moved to update
      if (currentAIState && currentAIState->getAnimCycleDuration() > 0) {
        animDelay = currentAIState->getAnimCycleDuration() / numChuteFrames;
      }

      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame = (currentFrame + 1) % numChuteFrames;
      }

      if (isOnGround) {
        isDescending = false;
        isLanding = true;
        if (payload) {
          payload->clearOverrideRect();
          payload->setPosX(posX);
          payload->setPosY(posY);
          payload->setTarget(target);
        }
      }
    } else if (isLanding) {
      applyPhysics();
      tileCollisionImpl(level);

      landTimer += dt;
      if (landTimer >= (5.0f / 60.0f)) {
        landTimer = 0;
        landFrame++;
        if (landFrame >= numLandFrames) {
          isLanding = false;
          landFrame = 0;
        }
      }
    } else {
      if (!payload)
        return;
      payload->setTarget(target);
      payload->setPosX(posX);
      payload->setPosY(posY);
      payload->update(level, dt);
      posX = payload->getPosX();
      posY = payload->getPosY();
      if (!payload->getIsAlive())
        isAlive = false;
      pendingShot = payload->getPendingShot();
      payload->clearPendingShot();
    }
  }

  void fireWeapon() override {
    if (!isDescending && !isLanding && payload)
      payload->fireWeapon();
  }

  void draw(RenderWindow &window) override {
    if (isDying) {
      InfantryEnemy::draw(window);
      return;
    }
    if (isDescending) {
      chuteSprite.setTextureRect(chuteFrames[currentFrame % numChuteFrames]);
      float chuteH = abs(chuteFrames[0].height) * 3;
      chuteSprite.setPosition(posX, posY - chuteH);
      window.draw(chuteSprite);
      if (payload) {
        payload->setPosX(posX);
        payload->setPosY(posY);
        payload->draw(window);
      }
    } else if (isLanding) {
      chuteSprite.setTextureRect(landFrames[landFrame % numLandFrames]);
      float chuteH = abs(chuteFrames[0].height) * 3;
      chuteSprite.setPosition(posX, posY - chuteH);
      window.draw(chuteSprite);
      if (payload) {
        payload->setPosX(posX);
        payload->setPosY(posY);
        payload->draw(window);
      }
    } else {
      if (payload)
        payload->draw(window);
    }
  }

  void activateSpecial() {}
  void applyCharacterBuffs() {}

private:
  void pickNewDrift() {
    float speed = 0.4f + (rand() % 60) / 100.0;
    driftX = (rand() % 2 == 0) ? speed : -speed;
    driftChangeTime = 1 + (rand() % 100) / 100.0;
    driftTimer = driftChangeTime;
  }

  void updateDrift(float dt) {
    driftTimer -= dt;
    if (driftTimer <= 0)
      pickNewDrift();
  }

  void updateAirShots(float dt) {
    if (airShotsRemaining <= 0 || !target || !weapon)
      return;

    airShotTimer -= dt;
    if (airShotTimer <= 0) {
      airShotTimer = airShotInterval + (rand() % 200) / 100.0f;
      float inaccuracy = ((rand() % 60) - 30) / 100.0f;
      double angle = M_PI / 2.0 + inaccuracy;
      double muzzleX = posX + getWidth() / 2.0;
      double muzzleY = posY + getHeight();

      pendingShot = weapon->fire(muzzleX, muzzleY, angle, false);
      if (pendingShot)
        airShotsRemaining--;
    }
  }
};