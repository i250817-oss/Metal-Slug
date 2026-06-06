#pragma once
#include "../Collectible and Interactable System/Fruit.h"
#include "../Collectible and Interactable System/Turkey.h"
#include <cstdlib>

// Enemy food drop logic (15% Turkey, 9% Fruit).
class EnemyDropSystem {
public:
  // Returns new Collectible* (PlayState takes ownership).
  static Collectible *roll(double dropX, double dropY) {
    int r = rand() % 100; // 0-99
    if (r < 15) {
      Collectible *t = new Turkey(dropX, dropY);
      t->setPosY(dropY - t->getHeight());
      return t;
    }
    if (r < 24) {
      Collectible *f = new Fruit(dropX, dropY);
      f->setPosY(dropY - f->getHeight());
      return f;
    }
    return nullptr;
  }
};