#pragma once
#include "Vehicle.h"



//abstract — vehicles that operate underwater or on water surface.
class AquaticVehicle : virtual public Vehicle {
private:
  double depth;
  bool isSubmerged;

public:
  AquaticVehicle() : Entity(), DamagableEntity(0,0,0), Vehicle(), depth(0.0), isSubmerged(false) {
    gravity = 0.05f;
    isFlying = true; // aquatic vehicles use aerial physics — no gravity sink
  }
  AquaticVehicle(double posX, double posY, int maxHp)
      : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Vehicle(posX, posY, maxHp, "aquatic"), depth(0.0), isSubmerged(false) {
    this->posX = posX;
    this->posY = posY;
    this->hp = maxHp;
    this->maxHp = maxHp;
    gravity = 0.05f;
    isFlying = true; // aquatic vehicles use aerial physics — no gravity sink
  }

  virtual ~AquaticVehicle() {}

public:
  void setDepth(double v) { depth = v; }
  double getDepth() const { return depth; }
  void setIsSubmerged(bool v) { isSubmerged = v; }
  bool getIsSubmerged() const { return isSubmerged; }

  virtual void loadAnimation(int type) override = 0;

  void update(Level *level, float dt) override {
    Vehicle::update(level, dt);
  }
};