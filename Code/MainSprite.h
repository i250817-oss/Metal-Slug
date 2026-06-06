#pragma once
#include "../headers/headers.h"

using namespace sf;



//mainSprite
//generic sprite sheet manager used by enemies and vehicles.
//players manage torso+leg sprites directly in PlayerSoldier.
class MainSprite {
private:
    float    widthOfSprites;
    float    heightOfSprites;
    int      numberOfSprites;
    int      posX;
    int      posY;
    Texture  mainImage;
    Sprite*  itsSprites;

public:
    MainSprite(float wos, float hos, int nos, const char* link, int posX, int posY)
        : widthOfSprites(wos), heightOfSprites(hos), numberOfSprites(nos)
        , posX(posX), posY(posY), itsSprites(nullptr)
    {
        mainImage.loadFromFile(link);
        itsSprites = new Sprite[nos];
        for (int i = 0; i < nos; i++) {
            itsSprites[i].setTexture(mainImage);
            itsSprites[i].setTextureRect(IntRect((i * wos), 0, wos, hos));
            itsSprites[i].setPosition(posX, posY);
        }
    }

    ~MainSprite() {
        delete[] itsSprites;
        itsSprites = nullptr;
    }

    void setWidthOfSprites(float v)  { widthOfSprites = v; }
    float getWidthOfSprites()  const { return widthOfSprites; }
    void setHeightOfSprites(float v) { heightOfSprites = v; }
    float getHeightOfSprites() const { return heightOfSprites; }
    void setNumberOfSprites(int v)   { numberOfSprites = v; }
    int  getNumberOfSprites()  const { return numberOfSprites; }

    Texture& getTexture() { return mainImage; }

    Sprite& getSprite(int index) { return itsSprites[index]; }

    void updateSprite(int x, int y) {
        for (int i = 0; i < numberOfSprites; i++)
            itsSprites[i].setPosition(x, y);
    }
};