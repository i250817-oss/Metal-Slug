#include "SupplyBox.h"
#include "../Character System/PlayerSoldier.h"
#include "../Weapon System/Weapon.h"
#include "../Weapon System/Ammo.h"

Texture SupplyBox::crateTexture;
bool    SupplyBox::textureLoaded = false;

void SupplyBox::onCollect(PlayerSoldier* player) {
    if (!player || isVanished) return;
    bool swapPressed = Keyboard::isKeyPressed(Keyboard::F);
    if (swapPressed && containedWeapon) {
        player->pickupWeapon(containedWeapon);
        containedWeapon = nullptr;
    } else if (containedAmmo) {
        player->replenishAmmo(containedAmmo->getQuantity());
    }
    player->setGrenadeCount(player->getGrenadeCount() + grenadeCount);
    isVanished = true;
}

void SupplyBox::update(Level* level, float dt) {
    Collectible::update(level, dt);
    if (animFrame >= 6) animFrame = 0;
    crateSprite.setTextureRect(frames[animFrame]);
    crateSprite.setOrigin(0, 0); 
}
