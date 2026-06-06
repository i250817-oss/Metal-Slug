#include "Enemy.h"
#include "../Character System/PlayerSoldier.h"
#include "../Collectible and Interactable System/EnemyDropSystem.h"
#include "../Manager/EntityManager.h"
#include "../ScoreCard/ScoreCard.h"
#include "../Manager/SoundManager.h"

Enemy::~Enemy() {
  for (int i = 0; i < MAX_STATES; i++) {
    if (states[i]) {
      delete states[i];
      states[i] = nullptr;
    }
  }
}

void Enemy::playDeathSound() {
  if (deathSoundPlayed) return;
  deathSoundPlayed = true;

  std::string tag = enemyTag ? enemyTag : "";
  if (tag.find("boss") != std::string::npos || 
      tag == "sea_satan" || 
      tag == "iron_nokana" || 
      tag == "hairbuster") {
    return;
  }

  // Dynamic selection of original Metal Slug death grunts/screams
  int roll = rand() % 2;
  if (roll == 0) {
    SoundManager::playSound("sound/enemy_die_scream.ogg");
  } else {
    SoundManager::playSound("sound/enemy_die_groan.ogg");
  }
}

void Enemy::die() {
  isAlive = false;
  ScoreCard::reportEnemyKill(enemyTag, isAerialKill());
  if (!pendingDrop)
    pendingDrop = EnemyDropSystem::roll(posX, posY + getHeight());

  // Play boss heavy explosion!
  std::string tag = enemyTag ? enemyTag : "";
  if (tag.find("boss") != std::string::npos || 
      tag == "sea_satan" || 
      tag == "iron_nokana" || 
      tag == "hairbuster") {
    SoundManager::playSound("sound/heavy_explosion.ogg");
  } else {
    playDeathSound();
  }
}

float Enemy::distanceToPlayer() {
  target = EntityManager::getInstance()->getNearestPlayer(posX, posY);
  if (!target)
    return 9999.0f;
  float dx = target->getPosX() - posX;
  float dy = target->getPosY() - posY;
  return sqrt(dx * dx + dy * dy) / 64.0f;
}

double Enemy::getPlayerX() {
  if (!target)
    target = EntityManager::getInstance()->getNearestPlayer(posX, posY);
  return target ? target->getPosX() : posX;
}

double Enemy::getPlayerY() {
  if (!target)
    target = EntityManager::getInstance()->getNearestPlayer(posX, posY);
  return target ? target->getPosY() : posY;
}

void Enemy::update(Level *level, float dt) {
  if (!isAlive)
    return;

  // wall/ledge detection
  wallAhead = false;
  if (level) {
    int blockSize = level->getBlockSize();
    float probeX = isFacingLeft ? (posX - 4) : (posX + getWidth() + 4);
    int col = (int)(probeX / blockSize);
    int topRow = (int)(posY / blockSize);
    int bottomRow = (int)((posY + getHeight() - 4) / blockSize);

    for (int r = topRow; r <= bottomRow; r++) {
      if (level->isSolidAt(col, r)) {
        wallAhead = true;
        break;
      }
    }

    if (!wallAhead && isOnGround) {
      int floorRow = (int)((posY + getHeight() + 4) / blockSize);
      if (!level->isSolidAt(col, floorRow))
        wallAhead = true;
    }
  }

  if (currentAIState) {
    currentAIState->update(*this, dt);
  }
  applyPhysics();
  tileCollisionImpl(level);
}
