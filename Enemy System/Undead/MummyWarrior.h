#pragma once
#include "UndeadEnemy.h"

using namespace sf;
using namespace std;

// mummyWarrior
// 5 HP. Slow. Transforms player on collision. Crumbles when hit by bullets,
// resurrects after 3s. Only dies from fire or explosions.
// sprite: "Sprites/MUMMY_WARRIOR_1.png" (right-to-left frames)
class MummyWarrior : public UndeadEnemy {
private:
  IntRect walkFrames[22];
  IntRect infectFrames[28];
  int numWalkFrames, numInfectFrames;
  bool isCrumbled;
  float resurrectionTimer;
  bool isInfecting;

  // Death animation
  IntRect deathFrames[22];
  bool isDying;
  int deathFrame;
  float deathAnimTimer;

  static Texture mummyTexture;
  static bool mummyTextureLoaded;

public:
  MummyWarrior(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 5),
        UndeadEnemy(posX, posY, 5), numWalkFrames(22), numInfectFrames(28),
        isCrumbled(false), resurrectionTimer(0), isInfecting(false) {
    isDying = false;
    deathFrame = 0;
    deathAnimTimer = 0.0f;

    deathFrames[0]  = IntRect(1036, 1186, 31, 43);
    deathFrames[1]  = IntRect(989, 1185, 31, 44);
    deathFrames[2]  = IntRect(944, 1185, 32, 44);
    deathFrames[3]  = IntRect(899, 1188, 32, 41);
    deathFrames[4]  = IntRect(854, 1187, 33, 42);
    deathFrames[5]  = IntRect(805, 1181, 34, 48);
    deathFrames[6]  = IntRect(757, 1182, 34, 47);
    deathFrames[7]  = IntRect(711, 1174, 36, 55);
    deathFrames[8]  = IntRect(660, 1176, 39, 53);
    deathFrames[9]  = IntRect(606, 1172, 40, 57);
    deathFrames[10] = IntRect(555, 1173, 40, 56);
    deathFrames[11] = IntRect(506, 1174, 39, 55);
    deathFrames[12] = IntRect(455, 1175, 41, 54);
    deathFrames[13] = IntRect(404, 1176, 41, 53);
    deathFrames[14] = IntRect(352, 1177, 42, 52);
    deathFrames[15] = IntRect(303, 1181, 39, 48);
    deathFrames[16] = IntRect(253, 1181, 39, 48);
    deathFrames[17] = IntRect(204, 1181, 40, 48);
    deathFrames[18] = IntRect(155, 1182, 40, 47);
    deathFrames[19] = IntRect(106, 1183, 39, 46);
    deathFrames[20] = IntRect(58, 1184, 38, 45);
    deathFrames[21] = IntRect(11, 1184, 37, 45);

    walkSpeed = 0.8f;
    runSpeed = 1.2f;
    enemyTag = "mummy";

    addState(AI_STATE_PATROL, new PatrolState());
    addState(AI_STATE_CHASE, new ChaseState());
    changeState(AI_STATE_PATROL);

    if (!mummyTextureLoaded) {
      if (mummyTexture.loadFromFile("Sprites/Enemies/Enemy Mummy.png")) {
        mummyTextureLoaded = true;
      }
    }
    enemyTexture = mummyTexture;
    enemySprite.setTexture(enemyTexture);
    enemySprite.setScale(3, 3);
    animDelay = 6.0f / 60.0f;

    // walking (22 frames, right-to-left)
    walkFrames[0] = IntRect(883, 1010, 31, 45);
    walkFrames[1] = IntRect(844, 1010, 31, 45);
    walkFrames[2] = IntRect(805, 1010, 31, 45);
    walkFrames[3] = IntRect(767, 1011, 33, 44);
    walkFrames[4] = IntRect(722, 1012, 36, 43);
    walkFrames[5] = IntRect(680, 1011, 33, 44);
    walkFrames[6] = IntRect(638, 1010, 31, 45);
    walkFrames[7] = IntRect(593, 1010, 31, 45);
    walkFrames[8] = IntRect(548, 1010, 32, 45);
    walkFrames[9] = IntRect(508, 1010, 32, 45);
    walkFrames[10] = IntRect(471, 1010, 31, 45);
    walkFrames[11] = IntRect(432, 1011, 31, 44);
    walkFrames[12] = IntRect(394, 1012, 31, 43);
    walkFrames[13] = IntRect(355, 1011, 31, 44);
    walkFrames[14] = IntRect(314, 1010, 33, 45);
    walkFrames[15] = IntRect(275, 1010, 31, 45);
    walkFrames[16] = IntRect(235, 1010, 31, 45);
    walkFrames[17] = IntRect(195, 1010, 31, 45);
    walkFrames[18] = IntRect(144, 1010, 29, 45);
    walkFrames[19] = IntRect(106, 1010, 29, 45);
    walkFrames[20] = IntRect(68, 1010, 30, 45);
    walkFrames[21] = IntRect(27, 1010, 30, 45);

    // infecting / transforming player (28 frames)
    infectFrames[0] = IntRect(937, 1074, 28, 45);
    infectFrames[1] = IntRect(908, 1074, 21, 45);
    infectFrames[2] = IntRect(876, 1074, 23, 45);
    infectFrames[3] = IntRect(842, 1074, 25, 45);
    infectFrames[4] = IntRect(809, 1074, 26, 45);
    infectFrames[5] = IntRect(774, 1074, 26, 45);
    infectFrames[6] = IntRect(738, 1074, 27, 45);
    infectFrames[7] = IntRect(705, 1075, 27, 44);
    infectFrames[8] = IntRect(671, 1076, 27, 43);
    infectFrames[9] = IntRect(637, 1076, 28, 43);
    infectFrames[10] = IntRect(602, 1076, 28, 43);
    infectFrames[11] = IntRect(567, 1076, 29, 43);
    infectFrames[12] = IntRect(530, 1077, 30, 42);
    infectFrames[13] = IntRect(492, 1078, 30, 41);
    infectFrames[14] = IntRect(456, 1078, 30, 41);
    infectFrames[15] = IntRect(421, 1078, 29, 41);
    infectFrames[16] = IntRect(387, 1078, 28, 41);
    infectFrames[17] = IntRect(354, 1077, 28, 42);
    infectFrames[18] = IntRect(320, 1076, 28, 43);
    infectFrames[19] = IntRect(286, 1075, 27, 44);
    infectFrames[20] = IntRect(251, 1074, 26, 45);
    infectFrames[21] = IntRect(217, 1073, 25, 46);
    infectFrames[22] = IntRect(185, 1073, 25, 46);
    infectFrames[23] = IntRect(148, 1074, 29, 45);
    infectFrames[24] = IntRect(118, 1075, 23, 44);
    infectFrames[25] = IntRect(87, 1074, 23, 45);
    infectFrames[26] = IntRect(49, 1074, 27, 45);
    infectFrames[27] = IntRect(13, 1074, 28, 45);

    enemySprite.setTextureRect(walkFrames[0]);
  }

  ~MummyWarrior() {}
  int getScoreValue() const override { return 150; }

  float getWidth() const override { return 36 * 3; }
  float getHeight() const override { return 45 * 3; }

  void transformPlayer() override {
    if (target) {
      PlayerSoldier *p = target->getAsPlayerSoldier();
      if (p)
        p->transformToMummy();
    }
  }

  void crumble() {
    isCrumbled = true;
    resurrectionTimer = 3;
  }
  void resurrect() {
    isCrumbled = false;
    resurrectionTimer = 0;
  }

  bool getIsDying() const override { return isDying; }

  void takeDamage(int val) override {
    if (isDying) return;

    if (hp - val > 0) {
      UndeadEnemy::takeDamage(val);
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
    if (!isAlive) return;

    if (isDying) {
      deathAnimTimer += dt;
      float frameDelay = 6.0f / 60.0f;
      if (deathAnimTimer >= frameDelay) {
        deathAnimTimer = 0;
        deathFrame++;
      }
      if (deathFrame >= 22) {
        isDying = false;
        UndeadEnemy::takeDamage(9999);
        return;
      }
      applyPhysics();
      tileCollisionImpl(level);
      enemySprite.setTexture(enemyTexture);
      enemySprite.setTextureRect(deathFrames[deathFrame]);
      return;
    }

    if (isCrumbled) {
      resurrectionTimer -= dt;
      if (resurrectionTimer <= 0)
        resurrect();
      applyPhysics();
      tileCollisionImpl(level);
      return;
    }
    if (target) {
      float dx = (target->getPosX() - posX);
      float dy = (target->getPosY() - posY);
      float dist = sqrt(dx * dx + dy * dy);
      if (dist < getWidth()) {
        if (!isInfecting) {
          isInfecting = true;
          currentFrame = 0;
          animTimer = 0;
        }
        transformPlayer();
      }
    }

    // animation logic moved to update
    if (isInfecting) {
      animTimer += dt;
      if (animTimer >= (6.0f / 60.0f)) {
        animTimer = 0;
        currentFrame++;
        if (currentFrame >= numInfectFrames) {
          currentFrame = 0;
          isInfecting = false;
        }
      }
      enemySprite.setTextureRect(infectFrames[currentFrame % numInfectFrames]);
    } else {
      if (currentAIState && currentAIState->getAnimCycleDuration() > 0) {
        animDelay = currentAIState->getAnimCycleDuration() / numWalkFrames;
      }

      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame = (currentFrame + 1) % numWalkFrames;
      }
      enemySprite.setTextureRect(walkFrames[currentFrame % numWalkFrames]);
    }

    UndeadEnemy::update(level, dt);
  }

  void draw(RenderWindow &window) override {
    if (isCrumbled) {
      renderScaleY = 1.5f;
      enemySprite.setTextureRect(walkFrames[0]);
    } else {
      renderScaleY = 3.0f;
    }

    UndeadEnemy::draw(window);
  }
};
