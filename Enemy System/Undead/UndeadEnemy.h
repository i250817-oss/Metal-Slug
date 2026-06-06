#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../Enemy.h"

class UndeadEnemy : virtual public Enemy {
protected:
  Texture enemyTexture;
  Sprite enemySprite;
  float renderScaleY;

public:
  UndeadEnemy() : Entity(), DamagableEntity(0,0,0), Enemy(0,0,0), renderScaleY(3.0f) {}
  UndeadEnemy(double posX, double posY, int maxHp)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Enemy(posX, posY, maxHp), renderScaleY(3.0f) {
      this->posX = posX;
      this->posY = posY;
      this->hp = maxHp;
      this->maxHp = maxHp;
  }

  virtual ~UndeadEnemy() {}

  virtual void transformPlayer() override {
    if (target) {
      PlayerSoldier *p = target->getAsPlayerSoldier();
      if (p)
        p->transformToZombie();
    }
  }

  void update(Level *level, float dt) override {
    Enemy::update(level, dt);

    // if very close to player, transform them!
    if (distanceToPlayer() < 0.5f) {
      transformPlayer();
    }
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

      enemySprite.setScale(isFacingLeft ? 3.f : -3.f, renderScaleY);

      window.draw(enemySprite);
    }
  }
};