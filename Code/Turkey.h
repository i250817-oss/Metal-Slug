#pragma once
#include "Food.h"

using namespace sf;



//turkey
//concrete food item. Replenishes 3 saturation units on collection.
//small chance to drop from any enemy on death (spawned by Enemy::die()).
class Turkey : public Food
{
private:
    static Texture turkeyTexture;
    static bool        textureLoaded;
    IntRect        frames[25];

public:
    Turkey(double posX, double posY)
        : Food(posX, posY, 3.0)   //+3 saturation
    {
        //update AABB to match scaled size (approx 22x25 * 3 = 66x75)
        width = 64;
        height = 75;

        if (!textureLoaded) {
            if (turkeyTexture.loadFromFile("Sprites/Items.png")) textureLoaded = true;
        }
        foodSprite.setTexture(turkeyTexture);
        foodSprite.setScale(3, 3);

        frames[0] = IntRect(18, 1183, 10, 11);
        frames[1] = IntRect(33, 1182, 11, 12);
        frames[2] = IntRect(48, 1178, 12, 16);
        frames[3] = IntRect(64, 1174, 15, 19);
        frames[4] = IntRect(83, 1172, 16, 21);
        frames[5] = IntRect(104, 1170, 17, 23);
        frames[6] = IntRect(126, 1169, 19, 24);
        frames[7] = IntRect(151, 1168, 22, 25);
        frames[8] = IntRect(179, 1168, 22, 25);
        frames[9] = IntRect(205, 1168, 22, 25);
        frames[10] = IntRect(231, 1168, 21, 25);
        frames[11] = IntRect(257, 1168, 22, 25);
        frames[12] = IntRect(285, 1168, 22, 25);
        frames[13] = IntRect(312, 1168, 24, 25);
        frames[14] = IntRect(342, 1168, 22, 25);
        frames[15] = IntRect(369, 1168, 22, 25);
        frames[16] = IntRect(396, 1168, 21, 25);
        frames[17] = IntRect(421, 1168, 22, 25);
        frames[18] = IntRect(448, 1168, 22, 25);
        frames[19] = IntRect(474, 1168, 24, 25);
        frames[20] = IntRect(503, 1168, 23, 25);
        frames[21] = IntRect(532, 1167, 31, 28);
        frames[22] = IntRect(567, 1170, 31, 23);
        frames[23] = IntRect(603, 1173, 30, 20);
        frames[24] = IntRect(638, 1172, 30, 21);

        foodSprite.setTextureRect(frames[0]);
    }

    void update(Level* level, float dt) override
    {
        Food::update(level, dt);
        if (animFrame >= 25) animFrame = 0;
        foodSprite.setTextureRect(frames[animFrame]);
        foodSprite.setOrigin(0, 0); 
    }

    void draw(RenderWindow& window) override
    {
        if (isVanished) return;
        //center sprite horizontally in AABB
        float offsetX = (64.0f - (float)frames[animFrame].width * 3.0f) / 2.0f;
        foodSprite.setPosition(posX + offsetX, posY);
        window.draw(foodSprite);
    }
};

