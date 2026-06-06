#pragma once
#include "Food.h"

using namespace sf;




class Fruit : public Food {
private:
  static Texture fruitTexture;
  static bool textureLoaded;
  IntRect frames[24];

public:
  Fruit(double posX, double posY)
      : Food(posX, posY, 2.0) //+2 saturation
  {
    //update AABB to match scaled size (approx 20x20 * 3 = 60x60)
    width = 60;
    height = 60;

    if (!textureLoaded) {
      if (fruitTexture.loadFromFile("Sprites/Items.png"))
        textureLoaded = true;
    }
    foodSprite.setTexture(fruitTexture);
    foodSprite.setScale(3, 3);

    frames[0] = IntRect(15, 625, 16, 15);
    frames[1] = IntRect(35, 631, 19, 9);
    frames[2] = IntRect(58, 624, 16, 16);
    frames[3] = IntRect(78, 620, 10, 20);
    frames[4] = IntRect(93, 625, 16, 15);
    frames[5] = IntRect(112, 630, 19, 10);
    frames[6] = IntRect(134, 624, 15, 16);
    frames[7] = IntRect(154, 620, 10, 20);
    frames[8] = IntRect(171, 623, 18, 16);
    frames[9] = IntRect(193, 625, 20, 14);
    frames[10] = IntRect(217, 623, 19, 16);
    frames[11] = IntRect(241, 622, 18, 17);
    frames[12] = IntRect(263, 621, 16, 18);
    frames[13] = IntRect(283, 619, 14, 20);
    frames[14] = IntRect(301, 619, 16, 20);
    frames[15] = IntRect(321, 620, 17, 19);
    frames[16] = IntRect(343, 623, 18, 16);
    frames[17] = IntRect(366, 625, 20, 14);
    frames[18] = IntRect(390, 623, 19, 16);
    frames[19] = IntRect(413, 622, 18, 17);
    frames[20] = IntRect(435, 621, 16, 18);
    frames[21] = IntRect(455, 619, 14, 20);
    frames[22] = IntRect(473, 619, 16, 20);
    frames[23] = IntRect(493, 620, 17, 19);

    foodSprite.setTextureRect(frames[0]);
  }

  ~Fruit() {}

  void onCollect(PlayerSoldier *player) override { Food::onCollect(player); }

  void update(Level *level, float dt) override {
    Food::update(level, dt);
    if (animFrame >= 24)
      animFrame = 0;
    foodSprite.setTextureRect(frames[animFrame]);
    foodSprite.setOrigin(0, 0); 
  }

  void draw(RenderWindow& window) override {
    if (isVanished)
      return;
    //center sprite horizontally in AABB
    float offsetX = (60.0f - (float)frames[animFrame].width * 3.0f) / 2.0f;
    foodSprite.setPosition(posX + offsetX, posY);
    window.draw(foodSprite);
  }
};

