//playerVehicle.h
#pragma once
#include "Vehicle.h"



class PlayerVehicle : public virtual Vehicle {
protected:
    bool isOccupied;
    PlayerSoldier *driver;
    virtual void handleDriverInput() = 0;
public:
    PlayerVehicle() : Entity(), DamagableEntity(0,0,0), Vehicle(), isOccupied(false), driver(nullptr) {}
    PlayerVehicle(double posX, double posY, int maxHp, const char* biome)
        : Entity(posX, posY), DamagableEntity(posX, posY, maxHp), Vehicle(posX, posY, maxHp, biome), isOccupied(false), driver(nullptr) {
        this->posX = posX;
        this->posY = posY;
        this->hp = maxHp;
        this->maxHp = maxHp;
    }
    virtual ~PlayerVehicle() {}
    
    bool isPlayer() const override { return true; }
    bool isEnemy() const override { return false; }

    bool getIsOccupied() const override { return isOccupied; }
    PlayerSoldier* getDriver() const override { return driver; }
    void setDriver(PlayerSoldier* p) override {
        driver = p;
        isOccupied = (p != nullptr);
    }

    void update(Level *level, float dt) override {
        if (!isAlive) return;
        if (getIsOccupied()) {
            handleDriverInput();
        }
        Vehicle::update(level, dt);
    }
};