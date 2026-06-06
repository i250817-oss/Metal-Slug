#pragma once
#include "../Entity Root/DamagableEntity.h"
#include "BallisticProjectile.h"
#include "../Manager/SoundManager.h"
#include <cmath>

using namespace sf;



//rocket: Travels in a ballistic arc, creates a 3-block blast radius on impact.
class Rocket : public BallisticProjectile {
private:
    bool isExploding;
    int explosionTimer;
    float blastRadius;

    static const int ROCKET_HIT_CAP = 24;
    DamagableEntity* splashVictims[ROCKET_HIT_CAP];
    int splashVictimCount;

    //animation
    static Texture texture;
    static bool textureLoaded;
    static Texture blastTexture;
    static bool blastTextureLoaded;

    Sprite sprite;
    Sprite blastSprite;
    IntRect frames[26];
    IntRect blastFrames[15];

    int numFrames;
    int currentFrame;
    float animTimer;
    float animDelay;

public:
    Rocket(double posX, double posY, double angle, bool isPlayerOwned,
        float speed = 12, float damage = 5, bool isArc = true,
        bool affectedByTiles = true)
        : BallisticProjectile(posX, posY, angle, cos(angle) * speed,
            sin(angle) * speed, damage,
            isPlayerOwned, isArc, affectedByTiles),
        isExploding(false), explosionTimer(0),
        blastRadius(3 * 64), splashVictimCount(0),
        numFrames(26), currentFrame(0), animTimer(0), animDelay(2.0f / 60.0f)
    {
        entityType = ENTITY_PROJECTILE;
        gravity = 0.15f; //FIX: Lighter gravity for rockets than grenades
        for (int i = 0; i < ROCKET_HIT_CAP; i++) splashVictims[i] = nullptr;

        if (!textureLoaded) {
            if (texture.loadFromFile("Sprites/Marco Rossi 2.png")) {
                textureLoaded = true;
            }
        }
        if (!blastTextureLoaded) {
            if (blastTexture.loadFromFile("Sprites/Enemies/Rebel Soldier (Bazooka).png")) {
                blastTextureLoaded = true;
            }
        }

        sprite.setTexture(texture);
        sprite.setScale(2.5f, 2.5f);
        blastSprite.setTexture(blastTexture);
        blastSprite.setScale(4, 4);

        //rocket frames (left to right)
        frames[0]  = IntRect(919, 8877, 16, 38);
        frames[1]  = IntRect(940, 8877, 16, 35);
        frames[2]  = IntRect(961, 8877, 18, 37);
        frames[3]  = IntRect(984, 8877, 17, 35);
        frames[4]  = IntRect(1006, 8877, 21, 35);
        frames[5]  = IntRect(1032, 8877, 19, 35);
        frames[6]  = IntRect(1056, 8877, 23, 32);
        frames[7]  = IntRect(1084, 8877, 22, 30);
        frames[8]  = IntRect(1111, 8877, 29, 29);
        frames[9]  = IntRect(1145, 8877, 27, 27);
        frames[10] = IntRect(1177, 8877, 32, 24);
        frames[11] = IntRect(1215, 8877, 30, 23);
        frames[12] = IntRect(1250, 8877, 34, 22);
        frames[13] = IntRect(1289, 8877, 31, 20);
        frames[14] = IntRect(1325, 8877, 37, 18);
        frames[15] = IntRect(1367, 8877, 34, 17);
        frames[16] = IntRect(1406, 8877, 37, 17);
        frames[17] = IntRect(1448, 8877, 34, 17);
        frames[18] = IntRect(1487, 8877, 36, 19);
        frames[19] = IntRect(1528, 8878, 34, 18);
        frames[20] = IntRect(1567, 8877, 35, 22);
        frames[21] = IntRect(1607, 8879, 32, 20);
        frames[22] = IntRect(1644, 8877, 31, 24);
        frames[23] = IntRect(1680, 8878, 28, 23);
        frames[24] = IntRect(1713, 8878, 28, 30);
        frames[25] = IntRect(1746, 8881, 25, 27);

        //blast frames from Rebel Soldier (Bazooka).png
        blastFrames[0]  = IntRect(3, 771, 27, 27);
        blastFrames[1]  = IntRect(33, 773, 23, 23);
        blastFrames[2]  = IntRect(59, 774, 21, 21);
        blastFrames[3]  = IntRect(83, 775, 23, 20);
        blastFrames[4]  = IntRect(109, 772, 24, 24);
        blastFrames[5]  = IntRect(136, 770, 28, 27);
        blastFrames[6]  = IntRect(167, 768, 28, 29);
        blastFrames[7]  = IntRect(198, 768, 32, 29);
        blastFrames[8]  = IntRect(233, 767, 34, 30);
        blastFrames[9]  = IntRect(270, 766, 34, 30);
        blastFrames[10] = IntRect(307, 766, 35, 29);
        blastFrames[11] = IntRect(345, 765, 34, 30);
        blastFrames[12] = IntRect(382, 765, 36, 30);
        blastFrames[13] = IntRect(421, 765, 30, 22);
        blastFrames[14] = IntRect(454, 765, 32, 25);

        sprite.setTextureRect(frames[0]);
    }

    ~Rocket() {}

    bool isExplosiveProjectile() const override { return true; }

    //splash damage victim tracking — prevent double-hitting during explosion
    void onCollideWith(Entity* other, float dt) override {
        if (isExploding) {
            DamagableEntity *de = other->asTarget();
            if (de) {
                for (int i = 0; i < splashVictimCount; i++) {
                    if (splashVictims[i] == de) return;
                }
                if (splashVictimCount < ROCKET_HIT_CAP) {
                    splashVictims[splashVictimCount] = de;
                    splashVictimCount++;
                }
            }
        }
        other->resolveCollision(this, dt);
    }

    float getWidth() const override { return isExploding ? blastRadius : 16 * 2.5f; }
    float getHeight() const override { return isExploding ? blastRadius : 38 * 2.5f; }

    void onHit() override {
        if (!isExploding) {
            isExploding = true;
            SoundManager::playSound("sound/grenade_explosion.ogg");
            explosionTimer = 8;
            currentFrame = 0;
            animTimer = 0;
            animDelay = 2.0f / 60.0f;
            velocityX = 0;
            velocityY = 0;
            posX -= blastRadius / 2.0;
            posY -= blastRadius / 2.0;
        }
    }

    void update(Level* level, float dt) override {
        if (isExploding) {
            animTimer += dt;
            if (animTimer >= animDelay) {
                animTimer = 0;
                currentFrame++;
                if (currentFrame >= explosionTimer) die();
            }
        } else {
            BallisticProjectile::update(level, dt);
            animTimer += dt;
            if (animTimer >= animDelay) {
                animTimer = 0;
                currentFrame = (currentFrame + 1) % numFrames;
                sprite.setTextureRect(frames[currentFrame]);
            }
        }
    }

    void draw(RenderWindow& window) override {
        if (!isActive) return;

        if (isExploding) {
            if (currentFrame < 15) {
                blastSprite.setTextureRect(blastFrames[currentFrame % 15]);
                blastSprite.setPosition(posX, posY);
                window.draw(blastSprite);
            }
        } else {
            sprite.setPosition(posX, posY);
            window.draw(sprite);
        }
    }
};

//initialize static members
