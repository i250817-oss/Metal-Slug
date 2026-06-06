#pragma once
#include "../Entity Root/DamagableEntity.h"
#include "StraightProjectile.h"
#include <cmath>

using namespace sf;
using namespace std;

class EnergyBeam : public StraightProjectile {
private:
  float length;
  float duration;
  float timer;
  float width;
  float height;

  static const int BEAM_HIT_CAP = 32;
  DamagableEntity *victims[BEAM_HIT_CAP];
  int victimCount;

  static Texture texture;
  static bool textureLoaded;
  Sprite sprite;
  IntRect frames[9];
  int numFrames;
  int currentFrame;
  float animTimer;
  float animDelay;

public:
  EnergyBeam(double muzzleX, double muzzleY, double angle,
             bool isPlayerOwned = false)
      : StraightProjectile(muzzleX, muzzleY, angle, 0, 999, isPlayerOwned),
        length(900), duration(0.40f), timer(0.40f), width(0), height(0),
        victimCount(0), numFrames(9), currentFrame(0), animTimer(0),
        animDelay(2.0f / 60.0f) {
    entityType = ENTITY_PROJECTILE;
    for (int i = 0; i < BEAM_HIT_CAP; i++)
      victims[i] = nullptr;

    if (!textureLoaded) {
      if (texture.loadFromFile("Sprites/lazer.png"))
        textureLoaded = true;
    }
    sprite.setTexture(texture);

    sprite.setScale(length / 22.0f, 8.0f);

    frames[0] = IntRect(8, 32, 22, 8);
    frames[1] = IntRect(40, 33, 20, 5);
    frames[2] = IntRect(71, 33, 21, 6);
    frames[3] = IntRect(102, 33, 21, 6);
    frames[4] = IntRect(134, 33, 20, 5);
    frames[5] = IntRect(165, 33, 21, 6);
    frames[6] = IntRect(227, 10, 21, 8);
    frames[7] = IntRect(259, 9, 22, 9);
    frames[8] = IntRect(290, 10, 21, 8);

    sprite.setTextureRect(frames[0]);
    sprite.setOrigin(0, frames[0].height / 2.0f);
    sprite.setRotation(angle * 180.0f / M_PI);

    float beamThickness = 8.0f * 8.0f;
    float cosA = cos(angle);
    float sinA = sin(angle);

    if (abs(cosA) > 0.5f) { // Horizontal
      width = length;
      height = beamThickness;
      posY = muzzleY - (height / 2.0);
      if (cosA < 0)
        posX = muzzleX - length; // Left
      else
        posX = muzzleX; // Right
    } else {            // Vertical
      width = beamThickness;
      height = length;
      posX = muzzleX - (width / 2.0);
      if (sinA < 0)
        posY = muzzleY - length; // Up
      else
        posY = muzzleY; // Down
    }
  }

  ~EnergyBeam() {}

  float getWidth() const override { return width; }
  float getHeight() const override { return height; }

  void onHit() override {}

  void onCollideWith(Entity *other, float dt) override {
    DamagableEntity *de = other->asTarget();
    if (de) {
      for (int i = 0; i < victimCount; i++) {
        if (victims[i] == de)
          return;
      }
      if (victimCount < BEAM_HIT_CAP) {
        victims[victimCount] = de;
        victimCount++;
      }
    }
    other->resolveCollision(this, dt);
  }

  void update(Level *level, float dt) override {
    (void)level;
    timer -= dt;
    if (timer <= 0)
      die();

    animTimer += dt;
    if (animTimer >= animDelay) {
      animTimer = 0;
      currentFrame = (currentFrame + 1) % numFrames;
      sprite.setTextureRect(frames[currentFrame]);
      sprite.setOrigin(0, frames[currentFrame].height / 2.0f);
    }
  }

  void draw(RenderWindow &window) override {
    if (!isActive)
      return;
    float drawX, drawY;
    float cosA = cos(angle);
    float sinA = sin(angle);

    if (abs(cosA) > 0.5f) { // Horizontal
      drawX = (cosA < 0) ? posX + length : posX;
      drawY = posY + (height / 2.0);
    } else { // Vertical
      drawX = posX + (width / 2.0);
      drawY = (sinA < 0) ? posY + length : posY;
    }

    sprite.setPosition(drawX, drawY);
    window.draw(sprite);
  }
};
