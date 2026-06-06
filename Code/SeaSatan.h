#pragma once
#include "Boss.h"
#include "../../Entity Root/Entity.h"
#include "../../Entity Root/DamagableEntity.h"
#include "../../Projectile System/Rocket.h"
#include "../../Projectile System/Bullet.h"

using namespace sf;



//sea Satan (Aquatic Boss)
//armaments: 
//- Dual HMG guns (one on each side) -> Bullet
//- Rockets                          -> Rocket
//30 HP.
class SeaSatan : virtual public Boss {
private:
    float rocketCooldown;
    float hmgCooldown;
    int   hmgBurstCount;
    double retreatDestY;

    //animations
    bool  isLidOpening;
    float lidAnimTimer;
    int   lidFrame;
    bool  isTurretFiring;
    float turretAnimTimer;
    int   turretFrame;

    static Texture texture;
    static bool textureLoaded;
    
    Sprite hullSprite;
    Sprite lidSprite;
    Sprite turretSprite;

    IntRect hullStages[4];
    IntRect lidFrames[8];
    IntRect turretFrames[7];

public:
    SeaSatan(double posX, double posY)
        : Entity(posX, posY), DamagableEntity(posX, posY, 200),
          Boss(200, posX, posY, "sea_satan")
        , rocketCooldown(1.5f)
        , hmgCooldown(0.8f)
        , hmgBurstCount(0)
        , isLidOpening(false), lidAnimTimer(0), lidFrame(0)
        , isTurretFiring(false), turretAnimTimer(0), turretFrame(0)
    {
        if (!textureLoaded) {
            if (texture.loadFromFile("Sprites/Enemies/sea satan.png")) textureLoaded = true;
        }
        
        hullSprite.setTexture(texture);
        lidSprite.setTexture(texture);
        turretSprite.setTexture(texture);
        
        float scale = 2.5f;
        hullSprite.setScale(scale, scale);
        lidSprite.setScale(scale, scale);
        turretSprite.setScale(scale, scale);

        //hull Stages
        hullStages[0] = {2, 2, 336, 208};
        hullStages[1] = {340, 2, 336, 208};
        hullStages[2] = {2, 212, 336, 192};
        hullStages[3] = {340, 220, 336, 184};

        //lid Opening
        lidFrames[0] = {2, 668, 56, 32};
        lidFrames[1] = {60, 652, 56, 48};
        lidFrames[2] = {118, 652, 56, 48};
        lidFrames[3] = {176, 636, 56, 64};
        lidFrames[4] = {234, 636, 56, 64};
        lidFrames[5] = {292, 636, 64, 64};
        lidFrames[6] = {358, 636, 72, 64};
        lidFrames[7] = {432, 636, 72, 64};

        //turret Shooting
        turretFrames[0] = {2, 846, 72, 48};
        turretFrames[1] = {76, 838, 72, 56};
        turretFrames[2] = {150, 838, 72, 56};
        turretFrames[3] = {224, 830, 72, 64};
        turretFrames[4] = {298, 830, 72, 64};
        turretFrames[5] = {372, 830, 72, 64};
        turretFrames[6] = {446, 830, 72, 64};

        hullSprite.setTextureRect(hullStages[0]);
    }

    float getWidth()  const override { return 336 * 2.5f; }
    float getHeight() const override { return 208 * 2.5f; }
    int getScoreValue() const override { return 500; }

    void launchRocket() {
        if (!target) return;
        isLidOpening = true;
        lidFrame = 0;
        
        double mx = posX + getWidth() * 0.4;
        double my = posY + getHeight() * 0.1;
        double dx = target->getPosX() - mx;
        double dy = target->getPosY() - my;
        double angle = atan2(dy, dx);
        pendingShot = new Rocket(mx, my, angle, false, 9, 20, true, false);
    }

    void fireHMG() {
        if (!target) return;
        isTurretFiring = true;
        turretFrame = 0;

        double mx = posX + (isFacingLeft ? 0 : getWidth());
        double my = posY + getHeight() * 0.6;
        double dx = target->getPosX() - mx;
        double dy = target->getPosY() - my;
        double angle = atan2(dy, dx);
        pendingShot = new Bullet(mx, my, angle, false, 12, 1);
    }

    void spawnMinions() override {}
    void specialAttack() override { launchRocket(); }
    void fireWeapon() override { attack(0.016f); }

    void retreat() override {
        isRetreating = true;
        retreatDestY = posY + 800.0; // Dive down 800 pixels
    }

    void attack(float dt) override {
        if (rocketCooldown <= 0) {
            launchRocket();
            rocketCooldown = 1.5f;
        }
        if (hmgCooldown <= 0) {
            fireHMG();
            hmgBurstCount++;
            if (hmgBurstCount >= 5) {
                hmgBurstCount = 0; hmgCooldown = 1.5f;
            } else {
                hmgCooldown = 0.15f;
            }
        }
    }

    void move(float dt) override {
        if (!target || distanceToPlayer() > 20) {
            velocityX = 0;
            velocityY = 0;
            return;
        }

        double dx = target->getPosX() - (posX + getWidth()*0.5);
        velocityX = (dx > 0) ? walkSpeed : -walkSpeed;
        isFacingLeft = (dx < 0);

        //submarine vertical tracking with depth limit
        double dy = target->getPosY() - (posY + getHeight()*0.5);
        velocityY = (dy > 0) ? walkSpeed * 0.4 : -walkSpeed * 0.4;
        
        //keep within water (Surface ~3200, Floor ~3520)
        if (posY < 3100 && velocityY < 0) velocityY = 0;
        if (posY > 3450 && velocityY > 0) velocityY = 0;
    }

    void update(Level* level, float dt) override {
        if (!isAlive) return;
        if (shouldRetreat()) retreat();

        if (isRetreating) {
            velocityY = 4;
            posY += velocityY * dt * 60.0f;
            if (posY > retreatDestY) {
                isAlive = false;
                hasRetreated = true;
            }
            return;
        }

        if (rocketCooldown > 0) rocketCooldown -= dt;
        if (hmgCooldown > 0) hmgCooldown -= dt;

        //animate Lid
        if (isLidOpening) {
            lidAnimTimer += dt;
            if (lidAnimTimer >= 0.1f) {
                lidAnimTimer = 0; lidFrame++;
                if (lidFrame >= 8) { isLidOpening = false; lidFrame = 7; }
            }
        }

        //animate Turret
        if (isTurretFiring) {
            turretAnimTimer += dt;
            if (turretAnimTimer >= 0.08f) {
                turretAnimTimer = 0; turretFrame++;
                if (turretFrame >= 7) { isTurretFiring = false; turretFrame = 0; }
            }
        }

        move(dt);
        attack(dt);
        posX += velocityX * dt * 60.0f;
        posY += velocityY * dt * 60.0f;
    }

    void draw(RenderWindow& window) override {
        if (!isAlive) return;
        
        //damage stage
        int stage = 0;
        float hpPercent = (float)hp / maxHp;
        if (hpPercent < 0.25f) stage = 3;
        else if (hpPercent < 0.50f) stage = 2;
        else if (hpPercent < 0.75f) stage = 1;
        
        hullSprite.setTextureRect(hullStages[stage]);
        hullSprite.setPosition(posX, posY);
        
        //mirroring
        float scaleX = 2.5f;
        if (!isFacingLeft) {
            hullSprite.setScale(-scaleX, scaleX);
            hullSprite.setOrigin(336, 0);
        } else {
            hullSprite.setScale(scaleX, scaleX);
            hullSprite.setOrigin(0, 0);
        }

        window.draw(hullSprite);

        //draw Lid
        if (isLidOpening || lidFrame > 0) {
            lidSprite.setTextureRect(lidFrames[lidFrame]);
            lidSprite.setPosition(posX + (isFacingLeft ? 100 : 450), posY - 20);
            window.draw(lidSprite);
        }

        //draw Turret
        turretSprite.setTextureRect(turretFrames[turretFrame]);
        turretSprite.setPosition(posX + (isFacingLeft ? 0 : 700), posY + 100);
        window.draw(turretSprite);
    }
};
