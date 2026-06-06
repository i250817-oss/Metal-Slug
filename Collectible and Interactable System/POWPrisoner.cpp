#include "POWPrisoner.h"
#include "../Character System/PlayerSoldier.h"
#include "../Level and Environment System/Level.h"

Texture POWPrisoner::prisonerTexture;
bool POWPrisoner::textureLoaded = false;

void POWPrisoner::setTarget(DamagableEntity *t) {
  if (!t) {
    nearbyPlayer = nullptr;
    return;
  }
  PlayerSoldier *p = t->getAsPlayerSoldier();
  if (p)
    nearbyPlayer = p;
}

void POWPrisoner::update(Level *level, float dt) {
  InteractableObject::update(level, dt);

  if (isVanished)
    return;

  if (state == POW_TIED && nearbyPlayer) {
    double playerCX = nearbyPlayer->getPosX() + nearbyPlayer->getWidth() * 0.5;
    double playerCY = nearbyPlayer->getPosY() + nearbyPlayer->getHeight() * 0.5;
    if (isInRange(playerCX, playerCY)) {
      if (Keyboard::isKeyPressed(Keyboard::E)) {
        onInteract(nearbyPlayer);
      }
    }
  }

  // animation state machine
  IntRect curRect;
  if (state == POW_TIED) {
    if (animFrame >= 9)
      animFrame = 0;
    curRect = tiedFrames[animFrame];
  } else if (state == POW_FREEING) {
    if (animFrame >= 7) {
      state = POW_FREED;
      animFrame = 0;
      curRect = freedFrames[0];
    } else {
      curRect = freeingFrames[animFrame];
    }
  } else if (state == POW_FREED) {
    if (animFrame >= 12) {
      isVanished = true;
      return;
    }
    curRect = freedFrames[animFrame];
    posX += 2.0; // run away to the right

    // flip sprite to face right (run-away direction)
    IntRect flipped = curRect;
    flipped.left += flipped.width;
    flipped.width = -flipped.width;
    prisonerSprite.setTextureRect(flipped);

    float offsetX = (99 - (float)curRect.width * 3) / 2.0f;
    prisonerSprite.setPosition(posX + offsetX, posY);
    prisonerSprite.setOrigin(0, 0);
    return; // already set texture and position
  }

  // shared path for TIED and FREEING states
  prisonerSprite.setTextureRect(curRect);

  float offsetX = (99 - curRect.width * 3) / 2.0f;
  prisonerSprite.setPosition(posX + offsetX, posY);
  prisonerSprite.setOrigin(0, 0);
}
