#pragma once
#include "biome.h"



class Block {
private:
    Biome* biome;        //which zone this block belongs to (owned externally)
    bool   destructible; //whether fire/explosions can destroy this block
    int    blockHealth;
    int    maxBlockHealth;
 
protected:
    //only BlockSub calls this
    Block(Biome* biome, bool destructible, int health = 1)
        : biome(biome), destructible(destructible), blockHealth(health), maxBlockHealth(health) {}
 
public:
    virtual ~Block() {}
 
    //ALL type queries pure virtual = 0
    virtual bool isAir()            const = 0;  //physics / pathfinding / spawn placement
    virtual bool isSolid()          const = 0;  //gravity, collision, standing logic
    virtual bool isWater()          const = 0;  //swim physics, underwater combat mode
    virtual bool isDeepWater()      const = 0;  //ship placement vs. dive logic
    virtual bool isSpawnMarker()    const = 0;  //read at level init, then cleared
    virtual bool isIndestructible() const = 0;
    virtual bool isMountainPeak()   const = 0;
    virtual bool isSeafloor()       const = 0;
    virtual bool isShipHull()       const = 0;
    virtual bool isPlatform()       const = 0;
    virtual bool isBarrel()         const = 0;  //destructible barrel prop
    virtual Block* clone()          const = 0;

    //block health / damage-stage
    //getDamageStage() returns 0=intact, 1=medium, 2=destroyed.
    //default returns 0 — non-barrel blocks ignore this.
    virtual int getDamageStage() const { return 0; }

    Block& operator-=(int amount) {
        blockHealth -= amount;
        if (blockHealth < 0) blockHealth = 0;
        return *this;
    }
    int getHealth()    const { return blockHealth;    }
    int getMaxHealth() const { return maxBlockHealth; }

    //rule of Three: copy-ctor added since operator= is user-defined
    Block(const Block& other)
        : biome(other.biome), destructible(other.destructible)
        , blockHealth(other.blockHealth), maxBlockHealth(other.maxBlockHealth) {}

    Block& operator=(const Block& other) {
        biome          = other.biome;
        destructible   = other.destructible;
        blockHealth    = other.blockHealth;
        maxBlockHealth = other.maxBlockHealth;
        return *this;
    }
 
    //Encapsulated mutable state
    bool isDestructible() const { return destructible; }
    void setDestructible(bool d){ destructible = d;    }
 
    //Biome association 
    Biome* getBiome() const { return biome; }
    void   setBiome(Biome* b) { biome = b; }
      
    bool isInAerial() const {
        if (biome == nullptr) return false;
        return biome->isAerial();
    }
    bool isInPlains() const {
        if (biome == nullptr) return false;
        return biome->isPlains();
    }
    bool isInAquatic() const {
        if (biome == nullptr) return false;
        return biome->isAquatic();
    }
 
    int getBlockSize() const {
        if (biome == nullptr) return 32;
        return biome->getBlockSize();
    }
    int getLevelMinWidth() const {
        if (biome == nullptr) return 150;
        return biome->getLevelMinWidth();
    }
    int getLevelMaxWidth() const {
        if (biome == nullptr) return 250;
        return biome->getLevelMaxWidth();
    }
    int getLevelMinHeight() const {
        if (biome == nullptr) return 20;
        return biome->getLevelMinHeight();
    }
    int getLevelMaxHeight() const {
        if (biome == nullptr) return 60;
        return biome->getLevelMaxHeight();
    }
};