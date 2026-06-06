//enemyVehicle.h
#pragma once
#include "Vehicle.h"



class EnemyVehicle : public virtual Vehicle {
protected:
    int fireMode;
    bool isPlayerFound;
    int missileRange;

public:
    EnemyVehicle() : Entity(), DamagableEntity(0,0,0), Vehicle(), fireMode(0), isPlayerFound(false), missileRange(600) {}
    EnemyVehicle(double posX, double posY, int maxHp, const char* biome)
        : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Vehicle(posX, posY, maxHp, biome), fireMode(0), isPlayerFound(false), missileRange(600) {
        this->posX = posX;
        this->posY = posY;
        this->hp = maxHp;
        this->maxHp = maxHp;
    }
    virtual ~EnemyVehicle() {}

    bool isPlayer() const override { return false; }
    bool isEnemy() const override { return true; }
    bool getIsOccupied() const override { return true; } //enemies are always "occupied"

    void setFireMode(int v) { fireMode = v; }
    int getFireMode() const { return fireMode; }
    void setIsPlayerFound(bool v) { isPlayerFound = v; }
    bool getIsPlayerFound() const { return isPlayerFound; }

    void update(Level *level, float dt) override {
        if (!isAlive) return;
        Vehicle::update(level, dt);
    }
};