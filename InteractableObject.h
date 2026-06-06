#pragma once
#include "Collectible.h"
#include <cmath>

using namespace sf;
using namespace std;

class InteractableObject : public Collectible {
protected:
  float interactionRadius;
  bool hasBeenInteracted;

public:
  InteractableObject(double posX, double posY, float radius, float w = 32,
                     float h = 64)
      : Collectible(posX, posY, -1, w, h), interactionRadius(radius),
        hasBeenInteracted(false) {}

  virtual ~InteractableObject() {}

  // getters
  float getInteractionRadius() const { return interactionRadius; }
  bool getHasBeenInteracted() const { return hasBeenInteracted; }

  // range check

  bool isInRange(double playerX, double playerY) const {
    double dx = (posX + width * 0.5) - playerX;
    double dy = (posY + height * 0.5) - playerY;
    double dist = sqrt(dx * dx + dy * dy);
    return dist <= interactionRadius;
  }

  // interaction hook

  virtual void onInteract(PlayerSoldier *player) = 0;

  void onCollect(PlayerSoldier *p) override {}

  void update(Level *level, float dt) override {
    Collectible::update(level, dt);
  }

  // draw
  virtual void draw(RenderWindow &window) override = 0;
};