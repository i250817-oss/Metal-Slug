#pragma once
#include "InteractableObject.h"
#include "SupplyBox.h"

using namespace sf;

const int POW_TIED = 0;
const int POW_FREEING = 1;
const int POW_FREED = 2;

class POWPrisoner : public InteractableObject {
private:
  int state;
  SupplyBox *pendingSpawn;
  static Texture prisonerTexture;
  static bool textureLoaded;
  Sprite prisonerSprite;
  PlayerSoldier *nearbyPlayer;

  IntRect tiedFrames[9];
  IntRect freeingFrames[7];
  IntRect freedFrames[12];

public:
  POWPrisoner(double posX, double posY)
      : InteractableObject(posX, posY, 96, 33 * 3, 45 * 3), state(POW_TIED),
        pendingSpawn(nullptr), nearbyPlayer(nullptr) {

    if (!textureLoaded) {
      if (prisonerTexture.loadFromFile("Sprites/NPCS and Prisoners.png"))
        textureLoaded = true;
    }
    prisonerSprite.setTexture(prisonerTexture);
    prisonerSprite.setScale(3, 3);

    // tied frames
    tiedFrames[0] = IntRect(6, 145, 33, 45);
    tiedFrames[1] = IntRect(42, 145, 34, 45);
    tiedFrames[2] = IntRect(79, 145, 35, 45);
    tiedFrames[3] = IntRect(117, 145, 34, 45);
    tiedFrames[4] = IntRect(154, 145, 36, 45);
    tiedFrames[5] = IntRect(193, 145, 38, 45);
    tiedFrames[6] = IntRect(234, 145, 39, 45);
    tiedFrames[7] = IntRect(276, 145, 43, 45);
    tiedFrames[8] = IntRect(322, 142, 44, 48);

    // freeing frames
    freeingFrames[0] = IntRect(6, 196, 45, 45);
    freeingFrames[1] = IntRect(54, 196, 46, 45);
    freeingFrames[2] = IntRect(106, 196, 43, 45);
    freeingFrames[3] = IntRect(154, 196, 20, 45);
    freeingFrames[4] = IntRect(179, 194, 26, 47);
    freeingFrames[5] = IntRect(210, 194, 33, 47);
    freeingFrames[6] = IntRect(293, 198, 40, 43);

    // freed frames
    freedFrames[0] = IntRect(6, 248, 33, 38);
    freedFrames[1] = IntRect(42, 249, 34, 37);
    freedFrames[2] = IntRect(79, 248, 29, 38);
    freedFrames[3] = IntRect(111, 245, 21, 40);
    freedFrames[4] = IntRect(135, 246, 19, 38);
    freedFrames[5] = IntRect(157, 247, 23, 39);
    freedFrames[6] = IntRect(183, 249, 30, 37);
    freedFrames[7] = IntRect(216, 248, 25, 38);
    freedFrames[8] = IntRect(244, 247, 22, 39);
    freedFrames[9] = IntRect(269, 246, 22, 40);
    freedFrames[10] = IntRect(294, 247, 30, 38);
    freedFrames[11] = IntRect(327, 247, 33, 38);

    prisonerSprite.setTextureRect(tiedFrames[0]);
    animDelay = 0.15f;
  }

  ~POWPrisoner() { delete pendingSpawn; }

  void setNearbyPlayer(PlayerSoldier *p) { nearbyPlayer = p; }
  void setTarget(DamagableEntity *t) override;
  Entity *getPendingSpawnEntity() override { return pendingSpawn; }
  void clearPendingSpawnEntity() override { pendingSpawn = nullptr; }

  void onInteract(PlayerSoldier *player) override {
    if (state != POW_TIED)
      return;
    state = POW_FREEING;
    animFrame = 0;
    animTimer = 0;
    hasBeenInteracted = true;

    pendingSpawn = new SupplyBox(posX + 64.0, posY + getHeight() - 96.0);
  }

  void update(Level *level, float dt) override;

  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }

  void resolveCollision(PlayerSoldier *p, float dt) override { onInteract(p); }

  void resolveCollision(Projectile *p, float dt) override {
    if (state == POW_TIED && p->getIsPlayerOwned()) {
      onInteract(nullptr);
    }
  }

  bool isHostileTarget() const override { return state == POW_TIED; }

  void draw(RenderWindow &window) override {
    if (isVanished)
      return;

    IntRect rect = prisonerSprite.getTextureRect();
    float offsetX = (99.0f - (float)rect.width * 3.0f) / 2.0f;
    prisonerSprite.setPosition(posX + offsetX, posY);
    window.draw(prisonerSprite);
  }
};
