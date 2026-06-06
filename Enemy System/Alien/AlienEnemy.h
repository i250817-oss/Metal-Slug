#pragma once
#include "../../Projectile System/EnergyBeam.h"
#include "../../Weapon System/FireArms/Pistol.h"
#include "../Enemy.h"

using namespace sf;
using namespace std;

// Martian: Phase 1 (Fly/Energy Beam) | Phase 2 (Pistol/On Foot)
class AlienEnemy : public Enemy {
private:
  Weapon *weapon;
  Texture enemyTexture;
  Sprite enemySprite;
  float enemyFireTimer;

  // Animations
  IntRect flyFrames[16];
  int numFlyFrames;

  IntRect podJumpFrames[14];
  int numPodJumpFrames;

  IntRect walkFrames[16];
  int numWalkFrames;

  IntRect fallFrames[9];
  int numFallFrames;

  IntRect shootFrames[18];
  int numShootFrames;

  IntRect deathFrames[41];
  bool isDying;
  int deathFrame;
  float deathAnimTimer;

  int phase;
  float beamCooldown;
  float beamChargeTimer;
  bool isBeamCharging;
  static const float BEAM_CHARGE_TIME;
  static const float HOVER_HEIGHT;

public:
  AlienEnemy(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 10),
        Enemy(posX, posY, 10), weapon(nullptr), enemyFireTimer(0.7f),
        numFlyFrames(16), numPodJumpFrames(14),
        numFallFrames(9), numShootFrames(18), phase(1),
        beamCooldown(4),
        beamChargeTimer(0), isBeamCharging(false) {
    isDying = false;
    deathFrame = 0;
    deathAnimTimer = 0.0f;

    deathFrames[0] = IntRect(10, 831, 48, 43);
    deathFrames[1] = IntRect(63, 831, 48, 43);
    deathFrames[2] = IntRect(116, 831, 49, 43);
    deathFrames[3] = IntRect(170, 831, 47, 43);
    deathFrames[4] = IntRect(225, 831, 50, 43);
    deathFrames[5] = IntRect(280, 831, 50, 43);
    deathFrames[6] = IntRect(335, 831, 46, 43);
    deathFrames[7] = IntRect(386, 831, 46, 43);
    deathFrames[8] = IntRect(437, 831, 47, 43);
    deathFrames[9] = IntRect(489, 831, 47, 43);
    deathFrames[10] = IntRect(541, 831, 49, 43);
    deathFrames[11] = IntRect(595, 832, 49, 42);
    deathFrames[12] = IntRect(649, 835, 49, 39);
    deathFrames[13] = IntRect(703, 833, 49, 41);
    deathFrames[14] = IntRect(757, 834, 48, 40);
    deathFrames[15] = IntRect(810, 837, 51, 37);
    deathFrames[16] = IntRect(10, 879, 51, 37);
    deathFrames[17] = IntRect(66, 879, 51, 37);
    deathFrames[18] = IntRect(122, 881, 51, 35);
    deathFrames[19] = IntRect(178, 882, 52, 34);
    deathFrames[20] = IntRect(235, 883, 52, 33);
    deathFrames[21] = IntRect(292, 885, 51, 31);
    deathFrames[22] = IntRect(348, 887, 51, 29);
    deathFrames[23] = IntRect(404, 893, 51, 23);
    deathFrames[24] = IntRect(460, 897, 51, 19);
    deathFrames[25] = IntRect(516, 896, 51, 20);
    deathFrames[26] = IntRect(572, 898, 51, 18);
    deathFrames[27] = IntRect(10, 926, 50, 22);
    deathFrames[28] = IntRect(65, 924, 48, 24);
    deathFrames[29] = IntRect(118, 922, 48, 26);
    deathFrames[30] = IntRect(171, 924, 48, 24);
    deathFrames[31] = IntRect(224, 933, 42, 15);
    deathFrames[32] = IntRect(277, 935, 41, 13);
    deathFrames[33] = IntRect(328, 936, 40, 12);
    deathFrames[34] = IntRect(382, 937, 32, 11);
    deathFrames[35] = IntRect(427, 938, 33, 10);
    deathFrames[36] = IntRect(470, 938, 36, 10);
    deathFrames[37] = IntRect(515, 940, 38, 8);
    deathFrames[38] = IntRect(558, 941, 43, 7);
    deathFrames[39] = IntRect(606, 943, 41, 5);
    deathFrames[40] = IntRect(654, 944, 34, 4);

    walkSpeed = 3.5f;
    runSpeed = 6.5f;
    enemyTag = "alien";

    addState(AI_STATE_PATROL, new PatrolState());
    addState(AI_STATE_CHASE, new ChaseState());
    addState(AI_STATE_ATTACK, new ShootState());
    addState(AI_STATE_RETREAT, new RetreatState());
    addState(AI_STATE_PANIC, new PanicState());
    changeState(AI_STATE_PATROL);
    if (enemyTexture.loadFromFile("Sprites/Enemies/Martian.png")) {
      enemySprite.setTexture(enemyTexture);
      enemySprite.setScale(3, 3);
    }
    animDelay = 5.0f / 60.0f;

    // Phase 1: Pod
    flyFrames[0] = IntRect(842, 80, 48, 42);
    flyFrames[1] = IntRect(789, 80, 48, 42);
    flyFrames[2] = IntRect(736, 80, 48, 42);
    flyFrames[3] = IntRect(683, 80, 48, 42);
    flyFrames[4] = IntRect(627, 80, 50, 42);
    flyFrames[5] = IntRect(570, 80, 52, 42);
    flyFrames[6] = IntRect(513, 80, 52, 42);
    flyFrames[7] = IntRect(456, 80, 52, 42);
    flyFrames[8] = IntRect(399, 80, 52, 42);
    flyFrames[9] = IntRect(343, 80, 51, 42);
    flyFrames[10] = IntRect(287, 80, 51, 42);
    flyFrames[11] = IntRect(230, 80, 52, 42);
    flyFrames[12] = IntRect(174, 80, 51, 42);
    flyFrames[13] = IntRect(119, 80, 50, 42);
    flyFrames[14] = IntRect(64, 79, 50, 43);
    flyFrames[15] = IntRect(10, 79, 49, 43);

    // Phase 1: Descent
    podJumpFrames[0] = IntRect(10, 202, 47, 44);
    podJumpFrames[1] = IntRect(62, 202, 44, 44);
    podJumpFrames[2] = IntRect(111, 202, 42, 44);
    podJumpFrames[3] = IntRect(158, 202, 45, 44);
    podJumpFrames[4] = IntRect(208, 204, 46, 42);
    podJumpFrames[5] = IntRect(259, 206, 45, 40);
    podJumpFrames[6] = IntRect(309, 207, 43, 39);
    podJumpFrames[7] = IntRect(357, 208, 40, 38);
    podJumpFrames[8] = IntRect(402, 210, 37, 36);
    podJumpFrames[9] = IntRect(444, 213, 35, 33);
    podJumpFrames[10] = IntRect(484, 206, 30, 40);
    podJumpFrames[11] = IntRect(519, 206, 43, 40);
    podJumpFrames[12] = IntRect(567, 206, 35, 37);
    podJumpFrames[13] = IntRect(607, 206, 31, 34);

    // Phase 2: Fall
    fallFrames[0] = IntRect(281, 264, 28, 27);
    fallFrames[1] = IntRect(247, 264, 29, 27);
    fallFrames[2] = IntRect(214, 264, 28, 27);
    fallFrames[3] = IntRect(180, 264, 29, 27);
    fallFrames[4] = IntRect(147, 264, 28, 27);
    fallFrames[5] = IntRect(113, 264, 29, 27);
    fallFrames[6] = IntRect(80, 264, 28, 27);
    fallFrames[7] = IntRect(45, 264, 30, 27);
    fallFrames[8] = IntRect(11, 264, 29, 27);

    // Phase 2: Shoot
    shootFrames[0] = IntRect(10, 309, 49, 43);
    shootFrames[1] = IntRect(64, 309, 49, 43);
    shootFrames[2] = IntRect(118, 309, 47, 43);
    shootFrames[3] = IntRect(170, 309, 47, 43);
    shootFrames[4] = IntRect(222, 309, 46, 43);
    shootFrames[5] = IntRect(273, 309, 46, 43);
    shootFrames[6] = IntRect(324, 309, 45, 43);
    shootFrames[7] = IntRect(374, 309, 44, 43);
    shootFrames[8] = IntRect(423, 309, 53, 43);
    shootFrames[9] = IntRect(481, 313, 55, 39);
    shootFrames[10] = IntRect(541, 314, 54, 38);
    shootFrames[11] = IntRect(379, 358, 53, 37);
    shootFrames[12] = IntRect(322, 359, 52, 36);
    shootFrames[13] = IntRect(262, 360, 55, 35);
    shootFrames[14] = IntRect(167, 359, 67, 36);
    shootFrames[15] = IntRect(84, 358, 70, 37);
    shootFrames[16] = IntRect(10, 357, 69, 38);
    shootFrames[17] = IntRect(10, 400, 53, 42);

    // Populate walkFrames with standing/walking out-of-pod frames (using first
    // 8 shoot frames as a proxy)
    for (int i = 0; i < 8; i++) {
      walkFrames[i] = shootFrames[i];
    }
    numWalkFrames = 8;

    // phase 1: float above ground
    isOnGround = true;
    gravity = 0;

    enemySprite.setTextureRect(flyFrames[0]);
  }

  ~AlienEnemy() { delete weapon; }

  float getWidth() const override { return 52 * 3; }
  float getHeight() const override { return 43 * 3; }

  bool getIsDying() const override { return isDying; }

  void fireWeapon() override {
    if (!weapon || enemyFireTimer > 0) return;
    double muzzleX = isFacingLeft ? posX : (posX + getWidth());
    double muzzleY = posY + getHeight() * 0.3;
    double angle = isFacingLeft ? M_PI : 0.0;
    pendingShot = weapon->fire(muzzleX, muzzleY, angle, false);
  }

  void takeDamage(int dmg) override {
    if (phase == 1) {
      if (hp - dmg <= 0) {
        hp = 1;
        isAlive = true;
        triggerPhase2();
      } else {
        hp -= dmg;
      }
      return;
    }
    if (isDying) return;
    if (hp - dmg > 0) {
      Enemy::takeDamage(dmg);
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

  void triggerPhase2() {
    phase = 2;
    gravity = 0.5f;
    hp = 9;
    isOnGround = false;
    weapon = new Pistol();
    enemyFireTimer = 0.7f;
    beamChargeTimer = 0;
    isBeamCharging = false;
    changeState(AI_STATE_CHASE);
  }

  void update(Level *level, float dt) override {
    if (!isAlive) return;

    if (isDying) {
      deathAnimTimer += dt;
      float frameDelay = 4.0f / 60.0f;
      if (deathAnimTimer >= frameDelay) {
        deathAnimTimer = 0;
        deathFrame++;
      }
      if (deathFrame >= 41) {
        isDying = false;
        Enemy::takeDamage(9999);
        return;
      }
      applyPhysics();
      tileCollisionImpl(level);
      enemySprite.setTexture(enemyTexture);
      enemySprite.setTextureRect(deathFrames[deathFrame]);
      return;
    }

    if (phase == 1) {
      if (target) {
        float tx = target->getPosX();
        float ty = target->getPosY() - HOVER_HEIGHT;
        float dx = tx - posX;
        float dy = ty - posY;
        if (abs(dx) > 5)
          posX += dx * 0.03;
        if (abs(dy) > 5)
          posY += dy * 0.02;

        float minY = target->getPosY() - 350;
        if (posY > minY)
          posY = minY;

        isFacingLeft = (dx < 0);

        beamCooldown -= dt;

        if (abs(dx) < 64 && beamCooldown <= 0) {
          isBeamCharging = true;
          beamChargeTimer += dt;

          if (beamChargeTimer >= BEAM_CHARGE_TIME) {
            double mX = posX + getWidth() / 2.0 - 5.0;
            double mY = posY + getHeight();
            pendingShot = new EnergyBeam(mX, mY, M_PI / 2.0, false);
            beamCooldown = 4;
            beamChargeTimer = 0;
            isBeamCharging = false;
          }
        } else {
          isBeamCharging = false;
          beamChargeTimer = 0;
        }
      }

      applyPhysics();
      tileCollisionImpl(level);
      if (weapon)
        weapon->tickCooldown(dt);
    } else {
      // Phase 2: standard ground enemy update
      Enemy::update(level, dt);
      if (enemyFireTimer > 0) enemyFireTimer -= dt;
      if (weapon) weapon->tickCooldown(dt);
    }

    // Animation Update Logic
    IntRect *frames = flyFrames;
    int maxFrames = numFlyFrames;
    float delay = 5.0f / 60.0f;

    if (phase == 2) {
      if (!isOnGround) {
        frames = fallFrames;
        maxFrames = numFallFrames;
      } else if (currentAIState && currentAIState->getID() == AI_STATE_ATTACK) {
        frames = shootFrames;
        maxFrames = numShootFrames;
        delay = 4.0f / 60.0f;
      } else {
        frames = walkFrames;
        maxFrames = numWalkFrames;
      }
    }

    if (currentAIState && currentAIState->getAnimCycleDuration() > 0) {
      delay = currentAIState->getAnimCycleDuration() / maxFrames;
    }

    animTimer += dt;
    if (animTimer >= delay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % maxFrames;
    }
    enemySprite.setTextureRect(frames[currentFrame % maxFrames]);
  }

  void draw(RenderWindow &window) override {
    if (enemySprite.getTexture()) {
      IntRect rect = enemySprite.getTextureRect();

      float w = abs(rect.width);
      float h = abs(rect.height);

      enemySprite.setOrigin(w / 2.0f, h);
      float sx = posX + getWidth() / 2.0f;
      float sy = posY + getHeight();
      enemySprite.setPosition(sx, sy);

      enemySprite.setScale(isFacingLeft ? 3.f : -3.f, 3.0f);

      window.draw(enemySprite);
    }
  }

  void activateSpecial() {}
  void applyCharacterBuffs() {}
  int getScoreValue() const override { return 200; }
};
