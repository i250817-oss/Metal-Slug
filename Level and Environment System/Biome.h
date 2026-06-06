#pragma once
#include "Block.h"

using namespace sf;



//abstract base for all biome regions.
//zone: 0=AERIAL, 1=PLAINS, 2=AQUATIC
class Biome {
private:
    int zone;
    int startX;
    int endX;
    int startY;
    int endY;
    Block** blocks;
    int blockCount;

public:
    Biome(int zone, int startX, int endX, int startY, int endY);
    virtual ~Biome();

    int getZone() const;
    int getStartX() const;
    int getEndX() const;
    int getStartY() const;
    int getEndY() const;

    Block* getBlock(int x, int y);
    void destroyBlock(int x, int y);

    virtual void generate() = 0;

    void updateRain(float dt, bool isRaining);
    void draw(RenderWindow& window);
};