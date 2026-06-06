#pragma once
#include "../../Character System/PlayerSoldier.h"
#include "../../Level and Environment System/Level.h"
#include "../../Projectile System/Bullet.h"
#include "../../Projectile System/Rocket.h"
#include "../Vehicle.h"

#include "MetalSlug.h"
#include "SlugFlyer.h"
#include "SlugMariner.h"

using namespace sf;

class AmphibiousSlug : virtual public MetalSlug,
                       virtual public SlugFlyer,
                       virtual public SlugMariner {
public:
  static const int MODE_GROUND = 0;
  static const int MODE_AIR = 1;
  static const int MODE_WATER = 2;

private:
  int currentMode;

public:
  AmphibiousSlug(double posX, double posY)
      : Entity(posX, posY), DamagableEntity(posX, posY, 150),
        Vehicle(posX, posY, 150, "all"), GroundVehicle(posX, posY, 150),
        AerialVehicle(posX, posY, 150), AquaticVehicle(posX, posY, 150),
        PlayerVehicle(posX, posY, 150, "all"), MetalSlug(posX, posY),
        SlugFlyer(posX, posY), SlugMariner(posX, posY) {
    currentMode = MODE_GROUND;
    loadAnimation(0);
  }

  virtual ~AmphibiousSlug() {}

  float getWidth() const override {
    if (currentMode == MODE_AIR) return 80 * 2.5f;
    if (currentMode == MODE_WATER) return 54 * 2.5f;
    return 62 * 2.5f;
  }

  float getHeight() const override {
    if (currentMode == MODE_AIR) return 46 * 2.5f;
    if (currentMode == MODE_WATER) return 52 * 2.5f;
    return 56 * 2.5f;
  }

  void switchVehicle(Level *level) {
    if (!level) return;
    int prevMode = currentMode;
    int biome = level->getBiomeAt(posX);

    if (biome == 1) currentMode = MODE_WATER;
    else if (biome == 2) currentMode = MODE_AIR;
    else currentMode = MODE_GROUND;

    if (currentMode != prevMode) {
      if (currentMode == MODE_GROUND) gravity = 0.5f;
      else if (currentMode == MODE_AIR) { gravity = 0; velocityY = 0; }
      else if (currentMode == MODE_WATER) gravity = 0.05f;
      
      loadAnimation(0);
    }
  }

  void loadAnimation(int dummy) override {
    if (currentMode == MODE_GROUND) MetalSlug::loadAnimation(dummy);
    else if (currentMode == MODE_AIR) SlugFlyer::loadAnimation(dummy);
    else if (currentMode == MODE_WATER) SlugMariner::loadAnimation(dummy);
  }

  void handleDriverInput() override {
    if (currentMode == MODE_GROUND) MetalSlug::handleDriverInput();
    else if (currentMode == MODE_AIR) SlugFlyer::handleDriverInput();
    else if (currentMode == MODE_WATER) SlugMariner::handleDriverInput();
  }

  void update(Level *level, float dt) override {
    if (!isAlive) return;
    switchVehicle(level);
    
    if (getIsOccupied()) {
      handleDriverInput();
    }
    
    // Call the appropriate physics update based on mode
    if (currentMode == MODE_GROUND) GroundVehicle::update(level, dt);
    else if (currentMode == MODE_AIR) AerialVehicle::update(level, dt);
    else AquaticVehicle::update(level, dt);

    // Sync timers and shared state
    if (fireCooldown > 0) fireCooldown -= dt;
  }

  void draw(RenderWindow &window) override {
    if (!isAlive) return;
    if (currentMode == MODE_GROUND) MetalSlug::draw(window);
    else if (currentMode == MODE_AIR) SlugFlyer::draw(window);
    else if (currentMode == MODE_WATER) SlugMariner::draw(window);
  }
};