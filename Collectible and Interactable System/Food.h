#pragma once
#include "Collectible.h"

using namespace sf;

class Food : public Collectible {
protected:
  double hpLevelIncrement; // saturation units restored on pickup

  Texture foodTexture;
  Sprite foodSprite;

public:
  Food(double posX, double posY, double increment)
      : Collectible(posX, posY, 10, 32, 32) // 10-second auto-despawn
        ,
        hpLevelIncrement(increment) {}

  virtual ~Food() {}

  void setHpLevelIncrement(double v) { hpLevelIncrement = v; }
  double getHpLevelIncrement() const { return hpLevelIncrement; }

  void onCollect(PlayerSoldier *player) override;

  void update(Level *level, float dt) override {
    Collectible::update(level, dt);
  }

  virtual void draw(RenderWindow &window) override = 0;
};