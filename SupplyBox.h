#pragma once
#include "Collectible.h"
#include "../Weapon System/Weapon.h"
#include "../Weapon System/Ammo.h"
#include "../Weapon System/FireArms/HeavyMachineGun.h"
#include "../Weapon System/FireArms/RocketLauncher.h"
#include "../Weapon System/FireArms/FlameShot.h"
#include "../Weapon System/FireArms/LaserGun.h"
#include <cstdlib>   //rand()

using namespace sf;



// A supply crate containing a weapon, ammo, and grenades.
// Logic: Player can swap current weapon (key held) or just take ammo (key released).
// Weapon Odds: 90% split (HMG/Flame/Rocket), 10% Laser.
// Eri special: Converts Hand Grenades to Fire Bombs automatically.

//ammo quantities per weapon type in the crate
static const int CRATE_HMG_AMMO    = 100;
static const int CRATE_FLAME_AMMO  = 60;
static const int CRATE_ROCKET_AMMO = 10;
static const int CRATE_LASER_AMMO  = 5;

class SupplyBox : public Collectible
{
private:
    int     weaponTypeId;          //0=HMG 1=Flame 2=Rocket 3=Laser
    Weapon* containedWeapon;       //heap-allocated; SupplyBox owns it
    Ammo*   containedAmmo;         //heap-allocated; SupplyBox owns it
    int     grenadeCount;          //3-5 hand OR 1-3 fire bomb
    bool    isFireBombGrenades;    //true = fire bomb type

    static Texture crateTexture;
    static bool        textureLoaded;
    Sprite         crateSprite;
    IntRect        frames[6];

    //internal — roll random contents based on spec probabilities
    void generateContents()
    {
        //... (same weapon rolling logic)
        delete containedWeapon;
        delete containedAmmo;
        containedWeapon = nullptr;
        containedAmmo   = nullptr;
        int roll = rand() % 100;
        if (roll < 10) {
            weaponTypeId = 3;
            containedWeapon = new LaserGun();
            containedAmmo = new Ammo(CRATE_LASER_AMMO, 0, 0, "LaserAmmo");
        } else {
            int subRoll = rand() % 3;
            if (subRoll == 0) {
                weaponTypeId = 0;
                containedWeapon = new HeavyMachineGun();
                containedAmmo = new Ammo(CRATE_HMG_AMMO, 0, 3, "HMGAmmo");
            } else if (subRoll == 1) {
                weaponTypeId = 1;
                containedWeapon = new FlameShot();
                containedAmmo = new Ammo(CRATE_FLAME_AMMO, 0, 2, "FlameAmmo");
            } else {
                weaponTypeId = 2;
                containedWeapon = new RocketLauncher();
                containedAmmo = new Ammo(CRATE_ROCKET_AMMO, 0, 5, "RocketAmmo");
            }
        }
        int grenadeRoll = rand() % 2;
        if (grenadeRoll == 0) { isFireBombGrenades = false; grenadeCount = 3 + rand() % 3; }
        else { isFireBombGrenades = true; grenadeCount = 1 + rand() % 3; }
    }

public:
    SupplyBox(double posX, double posY)
        : Collectible(posX, posY, -1, 32 * 3, 32 * 3)
        , weaponTypeId(0)
        , containedWeapon(nullptr)
        , containedAmmo(nullptr)
        , grenadeCount(0)
        , isFireBombGrenades(false)
    {
        generateContents();

        if (!textureLoaded) {
            if (crateTexture.loadFromFile("Sprites/Items.png")) textureLoaded = true;
        }
        crateSprite.setTexture(crateTexture);
        crateSprite.setScale(3, 3);

        frames[0] = IntRect(16, 30, 32, 28);
        frames[1] = IntRect(53, 29, 32, 29);
        frames[2] = IntRect(90, 28, 32, 30);
        frames[3] = IntRect(127, 26, 32, 32);
        frames[4] = IntRect(165, 27, 32, 31);
        frames[5] = IntRect(203, 29, 32, 29);

        crateSprite.setTextureRect(frames[0]);
        animDelay = 0.12f; //slightly faster bob
    }

    ~SupplyBox() { delete containedWeapon; delete containedAmmo; }

    //getters
    Weapon* getContainedWeapon()   const { return containedWeapon;    }
    Ammo*   getContainedAmmo()     const { return containedAmmo;      }
    int     getGrenadeCount()      const { return grenadeCount;       }
    bool    getIsFireBombGrenades() const { return isFireBombGrenades; }

    void onCollect(PlayerSoldier* player) override;

    void update(Level* level, float dt) override;

    void draw(RenderWindow& window) override
    {
        if (isVanished) return;
        crateSprite.setPosition(posX, posY);
        window.draw(crateSprite);
    }
};

