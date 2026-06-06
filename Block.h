#pragma once
#include "../headers/headers.h"

using namespace sf;



//a single tile/voxel in the world.
//type: 0=DIRT, 1=STONE, 2=WATER, 3=INDESTRUCTIBLE, 4=AIR
class Block {
private:
    int type;
    int posX;
    int posY;
    bool isDestroyed;
    bool indestructible;   //bottom-most layer is always true
    bool hasWater;
    float waterFillTimer;  //time exposed to rain

public:
    Block(int type, int posX, int posY, bool indestructible = false);
    ~Block();

    void setType(int v);
    int getType() const;
    int getPosX() const;
    int getPosY() const;
    bool getIsDestroyed() const;
    bool getIndestructible() const;
    bool getHasWater() const;

    void destroy();                       //only if not indestructible
    void fillWithWater();                 //if crater <= 3 wide, >= 1 deep + rain
    void updateRainExposure(float dt);    //accumulates toward fillWithWater
    void draw(RenderWindow& window);
};