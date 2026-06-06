#include "Collectible.h"
#include "../Level and Environment System/Level.h"
#include "../Vehicle System/Vehicle.h"
#include "../Character System/PlayerSoldier.h"

void Collectible::resolveCollision(Vehicle *v, float dt) {
  if (v && v->getDriver()) {
    onCollect(v->getDriver());
  }
}

void Collectible::update(Level *level, float dt) {
  if (isVanished)
    return;

  // apply Gravity
  if (level) {
    velocityY += 0.5; // gravity constant
    if (velocityY > 20.0)
      velocityY = 20.0;

    double nextY = posY + velocityY;
    int col = posX / (float)level->getBlockSize();
    int row = (nextY + height) / (float)level->getBlockSize();

    if (level->isSolidAt(col, row)) {
      posY = row * level->getBlockSize() - height;
      velocityY = 0;
    } else {
      posY = nextY;
    }
  }

  // auto-despawn countdown (timeToRemain == -1 means never)
  if (timeToRemain > 0) {
    timeToRemain -= dt; 
    if (timeToRemain <= 0)
      isVanished = true;
  }

  // tick animation frame
  animTimer += dt;
  if (animTimer >= animDelay) {
    animTimer = 0;
    animFrame++;
  }
}

#include "Food.h"
void Food::onCollect(PlayerSoldier* player) {
    if (!player || isVanished) return;
    player->addSaturation(hpLevelIncrement);
    isVanished = true;
}
