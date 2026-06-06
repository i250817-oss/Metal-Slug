#pragma once
#include "../Core Engine And States/DamageOverlay.h"
#include "../Projectile System/Bullet.h"
#include "../Manager/SoundManager.h"
#include "../Projectile System/Grenade.h"
#include "../Weapon System/Explosives/HandGrenade.h"
#include "../Weapon System/FireArms/FireArms.h"
#include "../Weapon System/FireArms/FlameShot.h"
#include "../Weapon System/FireArms/HeavyMachineGun.h"
#include "../Weapon System/FireArms/LaserGun.h"
#include "../Weapon System/FireArms/Pistol.h"
#include "../Weapon System/FireArms/RocketLauncher.h"
#include "../Weapon System/Melee/Knife.h"
#include "../Weapon System/Weapon.h"
#include "Soldier.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// forward declarations
#include "../NEAT /NeatAgent.h"
class FusionCompanion;
class EntityManager;

// player base class with dual-sprite system
class PlayerSoldier : public Soldier {
protected:
  // active flag
  bool isActivePlayer;
  float baseMaxSpeed;

  // combat
  float pistolDamage;
  float knifeDamage;
  double pistolFireRate;
  double meleeRange;
  double meleeCooldown;
  int grenadeCount;
  int grenadeMax;
  int lives;

  Weapon *currentWeapon;
  Weapon *inventory[10];
  int inventorySize;
  int currentWeaponIndex;
  Explosives *grenadeWeapon;
  Vehicle *currentVehicle;
  bool oWasPressed;
  bool gWasPressed;
  bool fWasPressed;
  bool iWasPressed;
  bool kWasPressed;
  bool nWasPressed;
  bool pWasPressed;
  bool isInvincible;

  int scores;
  double saturation;
  bool isDead;

  // pending projectile
  int damageState;
  float damageStateTimer;
  DamageOverlay damageOverlay;

  bool specialActive;
  float specialDuration;
  bool isOccupied;
  bool tWasPressed;

public:
  Weapon *getCurrentWeapon() const { return currentWeapon; }
  void setIsOccupied(bool v) { isOccupied = v; }
  bool getIsOccupied() const { return isOccupied; }
  void setTWasPressed(bool v) { tWasPressed = v; }
  bool getTWasPressed() const { return tWasPressed; }

  void transformToZombie() {
    if (areEqual(getTransStateName(), "Undead"))
      return;
    changeTransState(new UndeadState());
    isConverting = true;
    conversionFrame = 0;
    conversionAnimTimer = 0;
  }
  void transformToMummy() {
    if (areEqual(getTransStateName(), "Mummy"))
      return;
    changeTransState(new MummyState());
    isConverting = true;
    conversionFrame = 0;
    conversionAnimTimer = 0;
  }

  float scaleFactor;
  int frameWidth;
  int frameHeight;  // torso reference height (idle frame height, pixels)
  float legOffsetY; // pixels from posY to where legs start (the "waist")
  float legOffsetX; // horizontal nudge for leg alignment
  float spawnX, spawnY;

  // animation Frame Buffers
  IntRect idleTorsoFrames[20], idleLegFrames[20];
  IntRect runTorsoFrames[20], runLegFrames[20];
  IntRect jumpTorsoFrames[20], jumpLegFrames[20];
  IntRect runJumpTorsoFrames[6], runJumpLegFrames[6]; // 3.3 / 3.4
  IntRect shootTorsoFrames[20];
  IntRect shootUpTorsoFrames[20];
  IntRect shootDownTorsoFrames[6]; // 4.7
  IntRect knifeTorsoFrames[20];
  IntRect runTorsoHMGFrames[20];
  IntRect shootTorsoHMGFrames[20];
  IntRect shootUpTorsoHMGFrames[20];

  IntRect fastRunTorsoFrames[20], fastRunLegFrames[20];
  IntRect neatTorsoFrames[20], neatLegFrames[4];
  IntRect parachuteFlyingFrames[5];
  IntRect parachuteFallingFrames[17];
  Sprite parachuteSprite;

  // Crouch animations (4.8–4.12)
  IntRect crouchSitFrames[4];     // 4.8
  IntRect crouchIdleFrames[7];    // 4.9
  IntRect crouchSlideFrames[6];   // 4.10
  IntRect crouchFireFrames[10];   // 4.11
  IntRect slideFireFrames[10];     // 4.12
  IntRect pistolReloadFrames[19]; // 4.13
  IntRect standingHitFrames[8];   // 4.14
  IntRect crouchHitFrames[4];     // 4.15

  // HMG special
  IntRect hmgSlowTorsoFrames[4];       // 5.1
  IntRect hmgNeatTorsoFrames[12];      // 5.3
  IntRect hmgUpTransFrames[4];         // 5.4 (Fio uses 4)
  IntRect hmgFromUpTransFrames[4];     // 5.5 (Fio uses 4)
  IntRect hmgDownTransFrames[4];       // 5.8 (Fio uses 4)
  IntRect hmgFromDownTransFrames[12];  // 5.9 (Fio uses 12)
  IntRect hmgDownFireFrames[4];        // 5.10
  IntRect hmgLeftTransFrames[4];       // 5.11 (Fio uses 4)
  IntRect hmgLeftFireFrames[4];        // 5.12
  IntRect hmgHitFrames[8];             // 5.13
  IntRect hmgCrouchSitFrames[4];       // 5.14 (Fio uses 4)
  IntRect hmgCrouchIdleFrames[4];      // 5.15
  IntRect hmgCrouchWalkFrames[7];      // 5.16
  IntRect hmgCrouchSlideFrames[5];     // 5.17
  IntRect hmgCrouchFireFrames[4];      // 5.18
  IntRect hmgCrouchSlideFireFrames[4]; // 5.19

  // Melee & Knife (6.1–6.14)
  IntRect frontMeleeFrames[6];  // 6.1
  IntRect jumpMeleeFrames[6];   // 6.2
  IntRect leftMeleeFrames[6];   // 6.3
  IntRect crouchMeleeFrames[9]; // 6.4
  IntRect knifeLoadFrames[6];   // 6.5
  // knifeTorsoFrames = 6.6
  IntRect crouchKnifeLoadFrames[4];   // 6.7
  IntRect crouchKnifeAttackFrames[6]; // 6.8
  IntRect crouchBoxerFrames[5];       // 6.9
  IntRect knifeStandLoadFrames[6];    // 6.10
  IntRect knifeStandKillFrames[6];    // 6.11
  IntRect crouchKnife2LoadFrames[4];  // 6.12
  IntRect crouchKnife2HitFrames[6];   // 6.13
  IntRect hmgPunchLoadFrames[5];      // 6.14

  // Special actions
  IntRect drinkWaterFrames[11]; // 7.1

  // Deaths
  IntRect dieNormalFrames[35];  // 8.1 (Fio uses 35)
  IntRect dieFireFrames[25];    // 8.2
  IntRect dieAlienFrames[35];   // 8.3
  IntRect dieAquaticFrames[20]; // 8.4

  // Fat/Motu animations
  IntRect motuConvertFrames[18]; // 9.1
  // motuWalk / motuLeg / motuFire etc use fatTorso*/fatLeg* already declared
  IntRect motuCrouchSitFrames[4];        // 9.4 (Fio uses 4)
  IntRect motuCrouchIdleFrames[6];       // 9.5
  IntRect motuCrouchMoveFrames[6];       // 9.6 (Fio uses 6)
  IntRect motuCrouchStandFrames[4];      // 9.7 (Fio uses 4)
  IntRect motuFireStraightFrames[8];     // 9.8
  IntRect motuFireUp45TransFrames[12];   // 9.9 (Fio uses 12)
  IntRect motuFireFromUp45Frames[12];    // 9.10 (Fio uses 12)
  IntRect motuFireUp90Frames[4];         // 9.11
  IntRect motuFireDown45TransFrames[12]; // 9.12 (Fio uses 12)
  IntRect motuFireFromDown45Frames[12];  // 9.13 (Fio uses 12)
  IntRect motuFireDownFrames[12];        // 9.14 (Fio uses 12)
  IntRect motuFireCrouchFrames[4];       // 9.15
  IntRect motuFireTopMultiFrames[12];    // 9.16 (Fio uses 12)
  IntRect motuFireBotMultiFrames[12];    // 9.17 (Fio uses 12)
  IntRect motuFireCrouchMultiFrames[8];  // 9.18
  IntRect motuEatFrames[31];             // 9.19
  IntRect motuHitFrames[12];             // 9.20 (Fio uses 12)
  IntRect motuCallFrames[16];            // 9.21 (Fio uses 16)
  IntRect motuDieFrames[13];             // 9.22

  // Zombie animations
  IntRect zombieConvertFrames[58]; // 10.1
  // zombieWalkFrames[12] already declared
  IntRect zombieHandStraightFrames[12];     // 10.3
  IntRect zombieHandFireFrames[12];         // 10.4
  IntRect zombieHandUpFrames[12];           // 10.5
  IntRect zombieHandUpFireFrames[10];       // 10.6
  IntRect zombieFastBodyFrames[24];         // 10.7
  IntRect zombieFastHandStraightFrames[24]; // 10.8
  IntRect zombieFastHandUpFrames[24];       // 10.9
  IntRect zombieJumpBodyFrames[16];         // 10.10
  IntRect zombieJumpHandStraightFrames[16]; // 10.11
  IntRect zombieJumpHandUpFrames[16];       // 10.12
  IntRect zombieDieFrames[76];              // 10.13

  // Mummy animations (11.x beyond what's declared)
  IntRect mummyConvertFrames[5];    // 11.1
  IntRect mummyFireDownFrames[8];   // 11.8
  IntRect mummyFireCrouchFrames[8]; // 11.9
  IntRect mummyCrouchSitFrames[6];  // 11.10
  IntRect mummyCrouchIdleFrames[3]; // 11.11
  IntRect mummyCrouchMoveFrames[8]; // 11.12
  IntRect mummyJumpLegFrames[6];    // 11.13
  IntRect mummyDieFrames[25];       // 11.14
  IntRect zombieConvert2Frames[58]; // 11.15

  // Aquatic / Swimming (12.x)
  IntRect swimEnterPistolFrames[3];             // 12.1
  IntRect swimEnterHMGFrames[3];                // 12.2
  IntRect swimLegStillStraightFrames[4];        // 12.3
  IntRect swimLegSwimStraightFrames[18];         // 12.4
  IntRect swimLegUpStraightFrames[4];           // 12.5
  IntRect swimLegStillSlightFrames[4];          // 12.6
  IntRect swimLegSwimSlightFrames[4];           // 12.7
  IntRect swimLegUpSlightFrames[4];             // 12.8
  IntRect swimLegStillMoreFrames[4];            // 12.9
  IntRect swimLegSwimMoreFrames[4];             // 12.10
  IntRect swimLegUpMoreFrames[4];               // 12.11
  IntRect swimLegFastFrames[12];                // 12.12
  IntRect swimUpperPistolStraightFrames[4];     // 12.13
  IntRect swimUpperPistolSlightFrames[4];       // 12.14
  IntRect swimUpperPistolMoreFrames[4];         // 12.15
  IntRect swimUpperHMGStraightFrames[4];        // 12.16
  IntRect swimUpperHMGSlightFrames[4];          // 12.17
  IntRect swimUpperHMGMoreFrames[4];            // 12.18
  IntRect swimFastUpperPistolStraightFrames[18]; // 12.19
  IntRect swimFastUpperPistolSlightFrames[8];   // 12.20
  IntRect swimFastUpperPistolMoreFrames[8];     // 12.21
  IntRect swimFastUpperHMGStraightFrames[8];    // 12.22
  IntRect swimFastUpperHMGSlightFrames[8];      // 12.23
  IntRect swimFastUpperHMGMoreFrames[8];        // 12.24
  IntRect swimFirePistolStraightFrames[10];     // 12.25
  IntRect swimFirePistolSlightFrames[10];       // 12.26
  IntRect swimFirePistolMoreFrames[10];         // 12.27
  IntRect swimFireHMGStraightFrames[4];         // 12.28
  IntRect swimFireHMGSlightFrames[4];           // 12.29
  IntRect swimFireHMGMoreFrames[4];             // 12.30
  IntRect swimTakeGunStraightFrames[16];        // 12.31
  IntRect swimTakeGunSlightFrames[16];          // 12.32
  IntRect swimTakeGunMoreFrames[16];            // 12.33

  int numIdleFrames, numRunFrames, numJumpFrames, numShootFrames;
  int numShootUpFrames, numKnifeFrames, numShootHMGFrames;
  int numJumpIntroFrames;
  int numFastRunFrames;
  int numNeatFrames;
  int numParachuteFlyingFrames;
  int numParachuteFallingFrames;

  // NEAT call animation state
  bool isNeatCalling;
  float neatCallTimer;

  IntRect zombieWalkFrames[12], zombieShootFrames[23];
  IntRect mummyTorsoIdleFrames[6], mummyLegIdleFrames[1];
  IntRect mummyTorsoWalkFrames[12], mummyLegWalkFrames[12];
  IntRect mummyShootFrontFrames[8], mummyShootUpFrames[8];

  IntRect fatTorsoIdleFrames[4], fatLegIdleFrames[1];
  IntRect fatTorsoWalkFrames[13], fatLegWalkFrames[12];
  IntRect fatTorsoShootFrames[8], fatTorsoShootUpFrames[7];

  Texture zombieSheet, mummySheet, fatSheet, parachuteSheet, marcoSheet;

  void transformToFat() {
    if (areEqual(getTransStateName(), "Fat"))
      return;
    changeTransState(new FatState());
    isConverting = true;
    conversionFrame = 0;
    conversionAnimTimer = 0;
  }

  bool isParachuting;
  int parachuteState; // 0 = flying, 1 = falling on ground
  float parachuteLandX;
  float parachuteLandY;
  float parachuteAnimTimer;
  int parachuteFrame;
  bool wasJumpingFromRun; // true when jump began at high horizontal speed
  bool isSwimming;        // true when in aquatic biome and underwater
  float swimAngle;        // 0=straight, 1=slight, 2=more (tilt level)

  // combat State
  float fireAnimTimer;
  float hitAnimTimer; // Added for hit reactions
  bool isDying;       // Added for death sequence
  int deathType;      // 0=Normal, 1=Fire, 2=Alien, 3=Aquatic
  int deathFrame;
  float deathAnimTimer;
  bool isConverting;
  int conversionFrame;
  float conversionAnimTimer;
  bool isLookingUp;

  float aimAngle;
  bool qWasPressed;
  bool wWasPressed;

  Sprite torsoSprite;
  Sprite legSprite;
  Texture mainSheet;
  IntRect *lastTorsoSet;
  IntRect *lastLegSet;
  NeatAgent *neatAgent;
  PlayerSoldier *companionTarget;

  void updateSpritePositions() {
    IntRect tRect = torsoSprite.getTextureRect();
    IntRect lRect = legSprite.getTextureRect();

    float tw = abs(tRect.width);
    float th = abs(tRect.height);
    float lw = abs(lRect.width);
    float lh = abs(lRect.height);

    torsoSprite.setOrigin(0, 0);
    torsoSprite.setScale(scaleFactor, scaleFactor);
    legSprite.setOrigin(0, 0);
    legSprite.setScale(scaleFactor, scaleFactor);

    float waistY = posY + legOffsetY;

    TransformationState *ts = getTransState();
    bool isZombie = (ts && ts->getID() == TRANS_STATE_UNDEAD);

    float baseLegH = abs(idleLegFrames[0].height) * scaleFactor;
    float feetY = posY + legOffsetY + baseLegH;

    float torsoH = th * scaleFactor;
    float torsoY = isZombie ? (feetY - torsoH) : (waistY - torsoH + 15);
    torsoSprite.setPosition(posX, torsoY);

    float legH = lh * scaleFactor;

    float tWidthBase = abs(tRect.width);
    float lWidthBase = abs(lRect.width);
    float finalLegOffsetX = legOffsetX;
    if (isFacingLeft)
      finalLegOffsetX = (tWidthBase - lWidthBase) * scaleFactor - legOffsetX;

    float finalLegX = posX + finalLegOffsetX;
    float finalLegY = waistY;
    if (isSwimming) {
      if (isFacingLeft) {
        finalLegX += 10.0f * scaleFactor;
      } else {
        finalLegX -= 10.0f * scaleFactor;
      }
      finalLegY -= 5.0f * scaleFactor;
    }
    legSprite.setPosition(finalLegX, finalLegY);
  }

  bool getIsInvincible() const { return isInvincible; }
  void restoreBaseMaxSpeed() override { max_speed = baseMaxSpeed; }
  void setImmortal(bool v) { isInvincible = v; }
  void unlockAllWeapons() {

    bool hasHMG = false, hasRocket = false, hasFlame = false, hasLaser = false;
    for (int i = 0; i < inventorySize; i++) {
      if (areEqual(inventory[i]->getWeaponName(), "Heavy Machine Gun"))
        hasHMG = true;
      if (areEqual(inventory[i]->getWeaponName(), "Rocket Launcher"))
        hasRocket = true;
      if (areEqual(inventory[i]->getWeaponName(), "Flame Shot"))
        hasFlame = true;
      if (areEqual(inventory[i]->getWeaponName(), "Laser Gun"))
        hasLaser = true;
    }
    if (!hasHMG && inventorySize < 10)
      inventory[inventorySize++] = new HeavyMachineGun();
    if (!hasRocket && inventorySize < 10)
      inventory[inventorySize++] = new RocketLauncher();
    if (!hasFlame && inventorySize < 10)
      inventory[inventorySize++] = new FlameShot();
    if (!hasLaser && inventorySize < 10)
      inventory[inventorySize++] = new LaserGun();
  }

  virtual void updateAnimation(float dt) {
    bool isMoving = (abs(velocityX) > 0.5f);
    bool isFiring = (fireAnimTimer > 0);
    bool isHMG = (currentWeapon && areEqual(currentWeapon->getWeaponName(),
                                            "Heavy Machine Gun"));
    bool isKnife =
        (currentWeapon && areEqual(currentWeapon->getWeaponName(), "Knife"));
    bool isCrouching = getIsCrouching();

    torsoSprite.setTexture(mainSheet);
    legSprite.setTexture(mainSheet);

    // --- NEAT CALL animation takes priority over everything ---
    if (isNeatCalling) {
      neatCallTimer -= dt;
      if (neatCallTimer <= 0.0f)
        isNeatCalling = false;

      if (lastTorsoSet != neatTorsoFrames) {
        currentFrame = 0;
        animTimer = 0;
        lastTorsoSet = neatTorsoFrames;
        lastLegSet = neatLegFrames;
      }
      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame++;
      }

      int upperIdx = currentFrame % numNeatFrames;
      int lowerIdx = (currentFrame / 4) % 4;

      IntRect tRect = neatTorsoFrames[upperIdx];
      IntRect lRect = neatLegFrames[lowerIdx];
      if (isFacingLeft) {
        tRect.left += tRect.width;
        tRect.width = -tRect.width;
        lRect.left += lRect.width;
        lRect.width = -lRect.width;
      }
      torsoSprite.setTextureRect(tRect);
      legSprite.setTextureRect(lRect);
      return;
    }

    bool isFastRunning = isMoving && abs(velocityX) > baseMaxSpeed * 1.05f;

    // ---- SWIMMING ANIMATION (Aquatic biome) ----
    if (isSwimming) {
      // Pick leg set by angle and movement
      IntRect *swimLeg;
      int swimLegCount = 4;
      if (isFastRunning) {
        swimLeg = swimLegFastFrames;
        swimLegCount = 12;
      } else if (abs(swimAngle) < 0.5f) {
        swimLeg =
            isMoving ? swimLegSwimStraightFrames : swimLegStillStraightFrames;
        swimLegCount = (swimLeg == swimLegSwimStraightFrames) ? 18 : 4;
      } else if (abs(swimAngle) < 1.2f) {
        swimLeg = isMoving ? swimLegSwimSlightFrames : swimLegStillSlightFrames;
      } else {
        swimLeg = isMoving ? swimLegSwimMoreFrames : swimLegStillMoreFrames;
      }

      // Pick torso set
      IntRect *swimTorso;
      int swimTorsoCount = 4;
      if (isFiring) {
        if (isHMG) {
          swimTorso = (abs(swimAngle) < 0.5f)   ? swimFireHMGStraightFrames
                      : (abs(swimAngle) < 1.2f) ? swimFireHMGSlightFrames
                                                : swimFireHMGMoreFrames;
        } else {
          swimTorso = (abs(swimAngle) < 0.5f)   ? swimFirePistolStraightFrames
                      : (abs(swimAngle) < 1.2f) ? swimFirePistolSlightFrames
                                                : swimFirePistolMoreFrames;
          swimTorsoCount = 10;
        }
      } else if (isMoving) {
        if (isHMG) {
          swimTorso = (abs(swimAngle) < 0.5f)   ? swimFastUpperHMGStraightFrames
                      : (abs(swimAngle) < 1.2f) ? swimFastUpperHMGSlightFrames
                                                : swimFastUpperHMGMoreFrames;
          swimTorsoCount = 8;
        } else {
          swimTorso =
              (abs(swimAngle) < 0.5f)   ? swimFastUpperPistolStraightFrames
              : (abs(swimAngle) < 1.2f) ? swimFastUpperPistolSlightFrames
                                        : swimFastUpperPistolMoreFrames;
          swimTorsoCount = (swimTorso == swimFastUpperPistolStraightFrames) ? 18 : 8;
        }
      } else {
        if (isHMG) {
          swimTorso = (abs(swimAngle) < 0.5f)   ? swimUpperHMGStraightFrames
                      : (abs(swimAngle) < 1.2f) ? swimUpperHMGSlightFrames
                                                : swimUpperHMGMoreFrames;
        } else {
          swimTorso = (abs(swimAngle) < 0.5f)   ? swimUpperPistolStraightFrames
                      : (abs(swimAngle) < 1.2f) ? swimUpperPistolSlightFrames
                                                : swimUpperPistolMoreFrames;
        }
      }

      if (swimTorso != lastTorsoSet || swimLeg != lastLegSet) {
        currentFrame = 0;
        animTimer = 0;
        lastTorsoSet = swimTorso;
        lastLegSet = swimLeg;
      }
      animTimer += dt;
      if (animTimer >= animDelay) {
        animTimer = 0;
        currentFrame++;
      }
      int ti = currentFrame % swimTorsoCount;
      int li = currentFrame % swimLegCount;

      IntRect tRect = swimTorso[ti];
      IntRect lRect = swimLeg[li];
      if (isFacingLeft) {
        tRect.left += tRect.width;
        tRect.width = -tRect.width;
        lRect.left += lRect.width;
        lRect.width = -lRect.width;
      }
      if (isMarco()) {
        torsoSprite.setTexture(parachuteSheet);
        legSprite.setTexture(parachuteSheet);
      } else {
        torsoSprite.setTexture(mainSheet);
        legSprite.setTexture(mainSheet);
      }
      torsoSprite.setTextureRect(tRect);
      legSprite.setTextureRect(lRect);
      return;
    }

    // ---- Resolve which torso/leg set to use ----
    IntRect *currentTorso = idleTorsoFrames;
    int maxTorsoFrames = numIdleFrames;
    IntRect *currentLeg = idleLegFrames;
    int maxLegFrames = numIdleFrames;

    // PRIORITY 0: Hit Reaction
    if (hitAnimTimer > 0) {
      if (isCrouching) {
        currentTorso = crouchHitFrames;
        maxTorsoFrames = 4;
      } else if (isHMG) {
        currentTorso = hmgHitFrames;
        maxTorsoFrames = 8;
      } else {
        currentTorso = standingHitFrames;
        maxTorsoFrames = 8;
      }
      currentLeg = idleLegFrames;
      maxLegFrames = 1;

      // PRIORITY 1: Crouching
    } else if (isCrouching) {
      // justCrouched = true while sit-down anim hasn't finished yet
      bool inSitAnim = (lastTorsoSet == crouchSitFrames ||
                        lastTorsoSet == hmgCrouchSitFrames);
      bool sitFinished =
          inSitAnim &&
          ((lastTorsoSet == crouchSitFrames && currentFrame >= 4) ||
           (lastTorsoSet == hmgCrouchSitFrames && currentFrame >= 3));
      bool neverCrouched = (!inSitAnim && lastTorsoSet != crouchIdleFrames &&
                            lastTorsoSet != crouchFireFrames &&
                            lastTorsoSet != crouchSlideFrames &&
                            lastTorsoSet != slideFireFrames &&
                            lastTorsoSet != hmgCrouchIdleFrames &&
                            lastTorsoSet != hmgCrouchWalkFrames &&
                            lastTorsoSet != hmgCrouchFireFrames &&
                            lastTorsoSet != hmgCrouchSlideFireFrames);
      bool justCrouched = neverCrouched || (inSitAnim && !sitFinished);

      if (isHMG) {
        if (justCrouched) {
          currentTorso = hmgCrouchSitFrames;
          maxTorsoFrames = 3;
        } else if (isFiring) {
          currentTorso =
              isMoving ? hmgCrouchSlideFireFrames : hmgCrouchFireFrames;
          maxTorsoFrames = 4;
        } else if (isMoving) {
          currentTorso = hmgCrouchWalkFrames;
          maxTorsoFrames = 7;
        } else {
          currentTorso = hmgCrouchIdleFrames;
          maxTorsoFrames = 4;
        }
      } else {
        if (justCrouched) {
          currentTorso = crouchSitFrames;
          maxTorsoFrames = 4;
        } else if (isFiring) {
          if (isKnife) {
            currentTorso = crouchMeleeFrames;
            maxTorsoFrames = isFio() ? 9 : 6;
          } else {
            currentTorso = isMoving ? slideFireFrames : crouchFireFrames;
            maxTorsoFrames = isMoving ? 5 : 10;
          }
        } else if (isMoving) {
          currentTorso = crouchSlideFrames;
          maxTorsoFrames = isFio() ? 6 : 5;
        } else {
          currentTorso = crouchIdleFrames;
          maxTorsoFrames = isFio() ? 4 : 7;
        }
      }
      // HIDE lower body completely when crouching
      currentLeg = idleLegFrames;
      maxLegFrames = 1;

      // PRIORITY 2: In the air
    } else if (!isOnGround) {
      // Track whether this jump was started at running speed
      if (jumpCount > 0 && wasJumpingFromRun == false) {
        wasJumpingFromRun = (abs(velocityX) > baseMaxSpeed * 0.7f);
      }
      bool useRunJump = wasJumpingFromRun && runJumpTorsoFrames[0].width > 0;
      if (useRunJump) {
        currentLeg = runJumpLegFrames;
        maxLegFrames = 6;
      } else {
        currentLeg = jumpLegFrames;
        maxLegFrames = numJumpFrames;
      }

      if (isFiring) {
        if (isKnife) {
          currentTorso = knifeTorsoFrames;
          maxTorsoFrames = numKnifeFrames;
        } else if (isHMG) {
          if (isLookingUp) {
            currentTorso = shootUpTorsoHMGFrames;
            maxTorsoFrames = numShootHMGFrames;
          } else if (aimAngle < -0.05f) {
            currentTorso = hmgDownFireFrames;
            maxTorsoFrames = 4;
          } else {
            currentTorso = shootTorsoHMGFrames;
            maxTorsoFrames = numShootHMGFrames;
          }
        } else {
          if (isLookingUp) {
            currentTorso = shootUpTorsoFrames;
            maxTorsoFrames = numShootUpFrames;
          } else if (aimAngle < -0.05f && shootDownTorsoFrames[0].width > 0) {
            currentTorso = shootDownTorsoFrames;
            maxTorsoFrames = 6;
          } else {
            currentTorso = shootTorsoFrames;
            maxTorsoFrames = numShootFrames;
          }
        }
      } else {
        if (useRunJump) {
          currentTorso = runJumpTorsoFrames;
          maxTorsoFrames = 6;
        } else {
          currentTorso = jumpTorsoFrames;
          maxTorsoFrames = numJumpFrames;
        }
      }

      // PRIORITY 3: Firing
    } else if (isFiring) {
      if (isKnife) {
        currentTorso = knifeTorsoFrames;
        maxTorsoFrames = numKnifeFrames;
      } else if (isHMG) {
        if (isLookingUp) {
          currentTorso = shootUpTorsoHMGFrames;
          maxTorsoFrames = numShootHMGFrames;
        } else if (aimAngle < -0.05f) {
          currentTorso = hmgDownFireFrames;
          maxTorsoFrames = 4;
        } else {
          currentTorso = shootTorsoHMGFrames;
          maxTorsoFrames = numShootHMGFrames;
        }
      } else {
        if (isLookingUp) {
          currentTorso = shootUpTorsoFrames;
          maxTorsoFrames = numShootUpFrames;
        } else if (aimAngle < -0.05f && shootDownTorsoFrames[0].width > 0) {
          currentTorso = shootDownTorsoFrames;
          maxTorsoFrames = 6;
        } else {
          currentTorso = shootTorsoFrames;
          maxTorsoFrames = numShootFrames;
        }
      }
      // Legs follow movement
      if (isFastRunning && numFastRunFrames > 1 && !isHMG) {
        currentLeg = fastRunLegFrames;
        maxLegFrames = numFastRunFrames;
      } else if (isMoving) {
        currentLeg = runLegFrames;
        maxLegFrames = numRunFrames;
      } else {
        currentLeg = idleLegFrames;
        maxLegFrames = numIdleFrames;
      }

      // PRIORITY 4: Fast run
    } else if (isFastRunning && numFastRunFrames > 1) {
      currentTorso = isHMG ? runTorsoHMGFrames : fastRunTorsoFrames;
      maxTorsoFrames = isHMG ? numRunFrames : numFastRunFrames;
      currentLeg = fastRunLegFrames;
      maxLegFrames = numFastRunFrames;

      // PRIORITY 5: Normal walk
    } else if (isMoving) {
      currentTorso = isHMG ? runTorsoHMGFrames : runTorsoFrames;
      maxTorsoFrames = numRunFrames;
      currentLeg = runLegFrames;
      maxLegFrames = numRunFrames;

      // PRIORITY 6: Idle
    } else {
      currentTorso = idleTorsoFrames;
      maxTorsoFrames = numIdleFrames;
      currentLeg = idleLegFrames;
      maxLegFrames = numIdleFrames;
    }

    // Reset frame counter when animation SET changes
    if (currentTorso != lastTorsoSet || currentLeg != lastLegSet) {
      currentFrame = 0;
      animTimer = 0;
      lastTorsoSet = currentTorso;
      lastLegSet = currentLeg;
    }

    float activeDelay = animDelay;
    if (isFiring) {
      if (isMarco()) {
        activeDelay *= (0.5f / 1.5f);
      } else {
        activeDelay *= 0.5f;
      }
    }
    animTimer += dt;
    if (animTimer >= activeDelay) {
      animTimer = 0;
      currentFrame++;
    }

    int torsoFrameIdx = currentFrame % maxTorsoFrames;
    int legFrameIdx = currentFrame % maxLegFrames;

    if ((currentTorso == crouchSitFrames ||
         currentTorso == hmgCrouchSitFrames) &&
        currentFrame >= maxTorsoFrames) {
      torsoFrameIdx = maxTorsoFrames - 1;
    }

    if (!isOnGround && (isTarma() || isMarco() || isEri())) {
      if (isFiring) {
        torsoFrameIdx = currentFrame % maxTorsoFrames;
      } else {
        torsoFrameIdx = (currentFrame >= maxTorsoFrames) ? (maxTorsoFrames - 1)
                                                         : currentFrame;
      }
      legFrameIdx =
          (currentFrame >= maxLegFrames) ? (maxLegFrames - 1) : currentFrame;
    } else if (isCrouching && (isTarma() || isMarco() || isEri()) &&
               !isFiring) {
      torsoFrameIdx = (currentFrame >= maxTorsoFrames) ? (maxTorsoFrames - 1)
                                                       : currentFrame;
    } else {
      // PING-PONG for standing jump
      if (!isOnGround && numJumpIntroFrames == 0 && numJumpFrames > 1 &&
          (currentLeg == jumpLegFrames || currentLeg == runJumpLegFrames)) {
        int n = maxLegFrames;
        int raw = currentFrame % n;
        int pIdx = (velocityY <= 0) ? raw : (n - 1 - raw);
        legFrameIdx = pIdx;
        if (!isTarma() && !isFiring) {
          torsoFrameIdx = pIdx;
        }
      }
      // One-shot intro then loop remainder
      else if (!isOnGround && numJumpIntroFrames > 0 &&
               currentLeg == jumpLegFrames) {
        if (currentFrame >= numJumpIntroFrames) {
          int loopSize = numJumpFrames - numJumpIntroFrames;
          if (loopSize > 0) {
            torsoFrameIdx = numJumpIntroFrames +
                            (currentFrame - numJumpIntroFrames) % loopSize;
            legFrameIdx = torsoFrameIdx;
          }
        }
      }
    }

    // Clamp to valid range
    if (torsoFrameIdx >= maxTorsoFrames)
      torsoFrameIdx = maxTorsoFrames - 1;
    if (legFrameIdx >= maxLegFrames)
      legFrameIdx = maxLegFrames - 1;

    IntRect tRect = currentTorso[torsoFrameIdx];
    IntRect lRect = currentLeg[legFrameIdx];

    // Horizontal flip via TextureRect
    if (isFacingLeft) {
      tRect.left += tRect.width;
      tRect.width = -tRect.width;
      lRect.left += lRect.width;
      lRect.width = -lRect.width;
    }

    torsoSprite.setTextureRect(tRect);
    if (isCrouching) {
      legSprite.setTextureRect(IntRect(0, 0, 0, 0));
    } else {
      legSprite.setTextureRect(lRect);
    }
  }

  void loadTransformationAssets(const char *mummySheetPath) {
    marcoSheet.loadFromFile("Sprites/Marco Rossi 1.png");
    // zOMBIE initialization (Shared across characters)
    if (zombieSheet.loadFromFile(
            "Sprites/Main Character Zombie Versions.png")) {
      zombieWalkFrames[0] = IntRect(10, 396, 43, 39);
      zombieWalkFrames[1] = IntRect(58, 396, 45, 39);
      zombieWalkFrames[2] = IntRect(108, 395, 47, 40);
      zombieWalkFrames[3] = IntRect(160, 395, 49, 40);
      zombieWalkFrames[4] = IntRect(214, 394, 50, 41);
      zombieWalkFrames[5] = IntRect(269, 394, 48, 41);
      zombieWalkFrames[6] = IntRect(322, 394, 45, 41);
      zombieWalkFrames[7] = IntRect(372, 395, 43, 40);
      zombieWalkFrames[8] = IntRect(421, 395, 41, 40);
      zombieWalkFrames[9] = IntRect(467, 396, 43, 39);
      zombieWalkFrames[10] = IntRect(10, 440, 44, 39);
      zombieWalkFrames[11] = IntRect(59, 440, 43, 39);

      zombieShootFrames[0] = IntRect(10, 874, 46, 39);
      zombieShootFrames[1] = IntRect(61, 873, 46, 40);
      zombieShootFrames[2] = IntRect(112, 874, 44, 39);
      zombieShootFrames[3] = IntRect(161, 874, 42, 39);
      zombieShootFrames[4] = IntRect(208, 872, 39, 41);
      zombieShootFrames[5] = IntRect(252, 871, 36, 42);
      zombieShootFrames[6] = IntRect(293, 872, 37, 41);
      zombieShootFrames[7] = IntRect(335, 874, 39, 39);
      zombieShootFrames[8] = IntRect(379, 874, 40, 39);
      zombieShootFrames[9] = IntRect(424, 874, 41, 39);
      zombieShootFrames[10] = IntRect(470, 874, 43, 39);
      zombieShootFrames[11] = IntRect(475, 923, 28, 42);
      zombieShootFrames[12] = IntRect(441, 923, 29, 42);
      zombieShootFrames[13] = IntRect(406, 923, 30, 42);
      zombieShootFrames[14] = IntRect(366, 923, 35, 42);
      zombieShootFrames[15] = IntRect(323, 924, 38, 41);
      zombieShootFrames[16] = IntRect(281, 925, 37, 40);
      zombieShootFrames[17] = IntRect(239, 925, 37, 40);
      zombieShootFrames[18] = IntRect(196, 926, 38, 39);
      zombieShootFrames[19] = IntRect(150, 928, 40, 37);
      zombieShootFrames[20] = IntRect(105, 929, 40, 36);
      zombieShootFrames[21] = IntRect(58, 927, 42, 38);
      zombieShootFrames[22] = IntRect(10, 926, 43, 39);

      // Initialize other zombie frames so they are available to all characters
      zombieHandStraightFrames[0] = IntRect(8, 6649, 35, 33);
      zombieHandStraightFrames[1] = IntRect(50, 6648, 36, 34);
      zombieHandStraightFrames[2] = IntRect(93, 6650, 35, 32);
      zombieHandStraightFrames[3] = IntRect(135, 6649, 37, 33);
      zombieHandStraightFrames[4] = IntRect(179, 6649, 35, 33);
      zombieHandStraightFrames[5] = IntRect(221, 6650, 35, 32);
      zombieHandStraightFrames[6] = IntRect(263, 6650, 35, 32);
      zombieHandStraightFrames[7] = IntRect(305, 6650, 35, 32);
      zombieHandStraightFrames[8] = IntRect(347, 6649, 35, 33);
      zombieHandStraightFrames[9] = IntRect(389, 6650, 36, 32);
      zombieHandStraightFrames[10] = IntRect(432, 6650, 36, 32);
      zombieHandStraightFrames[11] = IntRect(475, 6649, 35, 33);

      zombieHandFireFrames[0] = IntRect(8, 6689, 37, 33);
      zombieHandFireFrames[1] = IntRect(52, 6688, 38, 34);
      zombieHandFireFrames[2] = IntRect(97, 6690, 37, 32);
      zombieHandFireFrames[3] = IntRect(141, 6689, 38, 33);
      zombieHandFireFrames[4] = IntRect(186, 6689, 36, 33);
      zombieHandFireFrames[5] = IntRect(229, 6690, 36, 32);
      zombieHandFireFrames[6] = IntRect(272, 6690, 36, 32);
      zombieHandFireFrames[7] = IntRect(315, 6690, 36, 32);
      zombieHandFireFrames[8] = IntRect(358, 6689, 37, 33);
      zombieHandFireFrames[9] = IntRect(402, 6690, 38, 32);
      zombieHandFireFrames[10] = IntRect(447, 6690, 37, 32);
      zombieHandFireFrames[11] = IntRect(491, 6689, 36, 33);

      zombieHandUpFrames[0] = IntRect(9, 6729, 30, 48);
      zombieHandUpFrames[1] = IntRect(46, 6730, 27, 47);
      zombieHandUpFrames[2] = IntRect(80, 6731, 28, 46);
      zombieHandUpFrames[3] = IntRect(115, 6732, 29, 45);
      zombieHandUpFrames[4] = IntRect(151, 6731, 29, 46);
      zombieHandUpFrames[5] = IntRect(187, 6730, 31, 47);
      zombieHandUpFrames[6] = IntRect(225, 6730, 32, 47);
      zombieHandUpFrames[7] = IntRect(264, 6730, 32, 47);
      zombieHandUpFrames[8] = IntRect(303, 6728, 31, 49);
      zombieHandUpFrames[9] = IntRect(341, 6727, 30, 50);
      zombieHandUpFrames[10] = IntRect(378, 6727, 31, 50);
      zombieHandUpFrames[11] = IntRect(416, 6728, 30, 49);

      zombieHandUpFireFrames[0] = IntRect(8, 6784, 30, 48);
      zombieHandUpFireFrames[1] = IntRect(45, 6784, 28, 48);
      zombieHandUpFireFrames[2] = IntRect(80, 6785, 29, 47);
      zombieHandUpFireFrames[3] = IntRect(116, 6786, 30, 46);
      zombieHandUpFireFrames[4] = IntRect(153, 6785, 30, 47);
      zombieHandUpFireFrames[5] = IntRect(190, 6785, 32, 47);
      zombieHandUpFireFrames[6] = IntRect(229, 6784, 32, 48);
      zombieHandUpFireFrames[7] = IntRect(268, 6784, 32, 48);
      zombieHandUpFireFrames[8] = IntRect(307, 6783, 32, 49);
      zombieHandUpFireFrames[9] = IntRect(346, 6782, 31, 50);

      zombieFastBodyFrames[0] = IntRect(928, 5722, 25, 39);
      zombieFastBodyFrames[1] = IntRect(961, 5722, 24, 39);
      zombieFastBodyFrames[2] = IntRect(993, 5723, 26, 38);
      zombieFastBodyFrames[3] = IntRect(1026, 5725, 27, 36);
      zombieFastBodyFrames[4] = IntRect(1061, 5725, 28, 36);
      zombieFastBodyFrames[5] = IntRect(1098, 5725, 29, 36);
      zombieFastBodyFrames[6] = IntRect(1136, 5723, 30, 38);
      zombieFastBodyFrames[7] = IntRect(1174, 5722, 32, 39);
      zombieFastBodyFrames[8] = IntRect(1215, 5721, 34, 40);
      zombieFastBodyFrames[9] = IntRect(1257, 5721, 30, 40);
      zombieFastBodyFrames[10] = IntRect(1295, 5721, 28, 40);
      zombieFastBodyFrames[11] = IntRect(1332, 5722, 28, 39);
      zombieFastBodyFrames[12] = IntRect(1368, 5722, 28, 39);
      zombieFastBodyFrames[13] = IntRect(1403, 5722, 30, 39);
      zombieFastBodyFrames[14] = IntRect(1440, 5722, 29, 39);
      zombieFastBodyFrames[15] = IntRect(1477, 5721, 29, 40);
      zombieFastBodyFrames[16] = IntRect(1514, 5720, 29, 41);
      zombieFastBodyFrames[17] = IntRect(1550, 5720, 27, 41);
      zombieFastBodyFrames[18] = IntRect(1585, 5721, 27, 40);
      zombieFastBodyFrames[19] = IntRect(1618, 5724, 30, 37);
      zombieFastBodyFrames[20] = IntRect(1656, 5725, 34, 36);
      zombieFastBodyFrames[21] = IntRect(1696, 5724, 29, 37);
      zombieFastBodyFrames[22] = IntRect(1732, 5724, 23, 37);
      zombieFastBodyFrames[23] = IntRect(1765, 5723, 22, 38);

      zombieFastHandStraightFrames[0] = IntRect(929, 5769, 25, 11);
      zombieFastHandStraightFrames[1] = IntRect(964, 5769, 24, 12);
      zombieFastHandStraightFrames[2] = IntRect(997, 5770, 24, 12);
      zombieFastHandStraightFrames[3] = IntRect(1029, 5771, 23, 13);
      zombieFastHandStraightFrames[4] = IntRect(1066, 5772, 21, 15);
      zombieFastHandStraightFrames[5] = IntRect(1104, 5773, 22, 14);
      zombieFastHandStraightFrames[6] = IntRect(1142, 5774, 23, 13);
      zombieFastHandStraightFrames[7] = IntRect(1182, 5777, 24, 10);
      zombieFastHandStraightFrames[8] = IntRect(1220, 5776, 24, 13);
      zombieFastHandStraightFrames[9] = IntRect(1262, 5776, 22, 14);
      zombieFastHandStraightFrames[10] = IntRect(1303, 5775, 21, 15);
      zombieFastHandStraightFrames[11] = IntRect(1339, 5774, 22, 13);
      zombieFastHandStraightFrames[12] = IntRect(1372, 5773, 24, 12);
      zombieFastHandStraightFrames[13] = IntRect(1408, 5772, 24, 12);
      zombieFastHandStraightFrames[14] = IntRect(1443, 5772, 25, 11);
      zombieFastHandStraightFrames[15] = IntRect(1478, 5772, 24, 12);
      zombieFastHandStraightFrames[16] = IntRect(1515, 5773, 24, 12);
      zombieFastHandStraightFrames[17] = IntRect(1552, 5774, 22, 13);
      zombieFastHandStraightFrames[18] = IntRect(1590, 5775, 21, 15);
      zombieFastHandStraightFrames[19] = IntRect(1625, 5776, 22, 14);
      zombieFastHandStraightFrames[20] = IntRect(1664, 5777, 19, 12);
      zombieFastHandStraightFrames[21] = IntRect(1701, 5774, 22, 13);
      zombieFastHandStraightFrames[22] = IntRect(1738, 5773, 17, 12);
      zombieFastHandStraightFrames[23] = IntRect(1766, 5772, 21, 12);

      zombieFastHandUpFrames[0] = IntRect(935, 5797, 11, 25);
      zombieFastHandUpFrames[1] = IntRect(969, 5798, 12, 24);
      zombieFastHandUpFrames[2] = IntRect(1003, 5798, 12, 24);
      zombieFastHandUpFrames[3] = IntRect(1033, 5799, 13, 23);
      zombieFastHandUpFrames[4] = IntRect(1068, 5800, 15, 21);
      zombieFastHandUpFrames[5] = IntRect(1106, 5799, 14, 22);
      zombieFastHandUpFrames[6] = IntRect(1146, 5798, 13, 23);
      zombieFastHandUpFrames[7] = IntRect(1186, 5797, 10, 24);
      zombieFastHandUpFrames[8] = IntRect(1223, 5797, 13, 24);
      zombieFastHandUpFrames[9] = IntRect(1264, 5799, 14, 22);
      zombieFastHandUpFrames[10] = IntRect(1304, 5800, 15, 21);
      zombieFastHandUpFrames[11] = IntRect(1344, 5799, 12, 24);
      zombieFastHandUpFrames[12] = IntRect(1376, 5797, 12, 25);
      zombieFastHandUpFrames[13] = IntRect(1411, 5796, 12, 28);
      zombieFastHandUpFrames[14] = IntRect(1447, 5797, 13, 26);
      zombieFastHandUpFrames[15] = IntRect(1482, 5798, 12, 22);
      zombieFastHandUpFrames[16] = IntRect(1519, 5798, 12, 22);
      zombieFastHandUpFrames[17] = IntRect(1555, 5798, 13, 22);
      zombieFastHandUpFrames[18] = IntRect(1591, 5799, 15, 21);
      zombieFastHandUpFrames[19] = IntRect(1626, 5798, 14, 22);
      zombieFastHandUpFrames[20] = IntRect(1664, 5797, 14, 26);
      zombieFastHandUpFrames[21] = IntRect(1705, 5796, 12, 22);
      zombieFastHandUpFrames[22] = IntRect(1739, 5795, 12, 19);
      zombieFastHandUpFrames[23] = IntRect(1768, 5794, 12, 23);

      // Zombie jump body frames (from zombieSheet, Y=635)
      zombieJumpBodyFrames[0] = IntRect(10, 635, 40, 41);
      zombieJumpBodyFrames[1] = IntRect(55, 635, 41, 42);
      zombieJumpBodyFrames[2] = IntRect(101, 635, 40, 43);
      zombieJumpBodyFrames[3] = IntRect(146, 635, 41, 44);
      zombieJumpBodyFrames[4] = IntRect(192, 635, 41, 45);
      zombieJumpBodyFrames[5] = IntRect(238, 635, 41, 45);
      zombieJumpBodyFrames[6] = IntRect(284, 635, 45, 42);

      zombieJumpHandStraightFrames[0] = IntRect(930, 5885, 24, 11);
      zombieJumpHandStraightFrames[1] = IntRect(964, 5885, 23, 11);
      zombieJumpHandStraightFrames[2] = IntRect(998, 5885, 24, 11);
      zombieJumpHandStraightFrames[3] = IntRect(1031, 5885, 24, 11);
      zombieJumpHandStraightFrames[4] = IntRect(1063, 5885, 24, 10);
      zombieJumpHandStraightFrames[5] = IntRect(1094, 5885, 24, 10);
      zombieJumpHandStraightFrames[6] = IntRect(1126, 5885, 25, 10);
      zombieJumpHandStraightFrames[7] = IntRect(1161, 5885, 25, 10);
      zombieJumpHandStraightFrames[8] = IntRect(1193, 5884, 25, 10);
      zombieJumpHandStraightFrames[9] = IntRect(1225, 5885, 25, 9);
      zombieJumpHandStraightFrames[10] = IntRect(1255, 5883, 25, 9);
      zombieJumpHandStraightFrames[11] = IntRect(1285, 5883, 25, 9);
      zombieJumpHandStraightFrames[12] = IntRect(1316, 5883, 25, 9);
      zombieJumpHandStraightFrames[13] = IntRect(1346, 5883, 25, 9);
      zombieJumpHandStraightFrames[14] = IntRect(1378, 5883, 23, 9);
      zombieJumpHandStraightFrames[15] = IntRect(1409, 5883, 25, 9);

      zombieJumpHandUpFrames[0] = IntRect(938, 5908, 11, 25);
      zombieJumpHandUpFrames[1] = IntRect(969, 5907, 11, 26);
      zombieJumpHandUpFrames[2] = IntRect(1003, 5906, 10, 27);
      zombieJumpHandUpFrames[3] = IntRect(1036, 5906, 10, 27);
      zombieJumpHandUpFrames[4] = IntRect(1069, 5906, 10, 27);
      zombieJumpHandUpFrames[5] = IntRect(1101, 5906, 10, 27);
      zombieJumpHandUpFrames[6] = IntRect(1134, 5906, 10, 27);
      zombieJumpHandUpFrames[7] = IntRect(1167, 5906, 10, 27);
      zombieJumpHandUpFrames[8] = IntRect(1201, 5906, 10, 27);
      zombieJumpHandUpFrames[9] = IntRect(1232, 5906, 10, 27);
      zombieJumpHandUpFrames[10] = IntRect(1262, 5906, 10, 27);
      zombieJumpHandUpFrames[11] = IntRect(1290, 5906, 10, 27);
      zombieJumpHandUpFrames[12] = IntRect(1319, 5906, 10, 21);
      zombieJumpHandUpFrames[13] = IntRect(1351, 5907, 11, 20);
      zombieJumpHandUpFrames[14] = IntRect(1383, 5907, 11, 20);
      zombieJumpHandUpFrames[15] = IntRect(1414, 5907, 11, 26);

      // Zombie conversion frames (from zombieSheet, Y=186-205)
      zombieConvertFrames[0] = IntRect(10, 186, 27, 40);
      zombieConvertFrames[1] = IntRect(41, 186, 31, 40);
      zombieConvertFrames[2] = IntRect(75, 186, 31, 40);
      zombieConvertFrames[3] = IntRect(112, 186, 30, 40);
      zombieConvertFrames[4] = IntRect(147, 186, 31, 40);
      zombieConvertFrames[5] = IntRect(183, 187, 32, 39);
      zombieConvertFrames[6] = IntRect(220, 189, 32, 37);
      zombieConvertFrames[7] = IntRect(255, 191, 29, 35);
      zombieConvertFrames[8] = IntRect(290, 193, 28, 33);
      zombieConvertFrames[9] = IntRect(323, 192, 27, 34);
      zombieConvertFrames[10] = IntRect(355, 194, 25, 32);
      zombieConvertFrames[11] = IntRect(386, 195, 24, 31);
      zombieConvertFrames[12] = IntRect(417, 197, 21, 29);
      zombieConvertFrames[13] = IntRect(447, 199, 22, 27);
      zombieConvertFrames[14] = IntRect(475, 201, 23, 25);
      zombieConvertFrames[15] = IntRect(503, 205, 27, 21);
    } else {
      cout << "ERROR: Failed to load Sprites/Main Character Zombie Versions.png"
           << endl;
    }

    if (mummySheet.loadFromFile(mummySheetPath)) {
      mummyLegIdleFrames[0] = IntRect(15, 107, 15, 25);
      mummyLegWalkFrames[0] = IntRect(15, 165, 15, 25);
      mummyLegWalkFrames[1] = IntRect(45, 165, 21, 25);
      mummyLegWalkFrames[2] = IntRect(81, 165, 25, 25);
      mummyLegWalkFrames[3] = IntRect(121, 165, 30, 25);
      mummyLegWalkFrames[4] = IntRect(166, 165, 31, 25);
      mummyLegWalkFrames[5] = IntRect(212, 165, 25, 24);
      mummyLegWalkFrames[6] = IntRect(252, 165, 22, 25);
      mummyLegWalkFrames[7] = IntRect(289, 165, 14, 25);
      mummyLegWalkFrames[8] = IntRect(318, 165, 21, 25);
      mummyLegWalkFrames[9] = IntRect(354, 165, 30, 25);
      mummyLegWalkFrames[10] = IntRect(399, 165, 25, 24);
      mummyLegWalkFrames[11] = IntRect(439, 165, 22, 25);

      mummyTorsoIdleFrames[0] = IntRect(15, 342, 35, 26);
      mummyTorsoIdleFrames[1] = IntRect(65, 342, 35, 26);
      mummyTorsoIdleFrames[2] = IntRect(115, 342, 35, 27);
      mummyTorsoIdleFrames[3] = IntRect(165, 342, 35, 28);
      mummyTorsoIdleFrames[4] = IntRect(215, 342, 35, 27);
      mummyTorsoIdleFrames[5] = IntRect(265, 342, 35, 26);

      mummyTorsoWalkFrames[0] = IntRect(15, 403, 35, 26);
      mummyTorsoWalkFrames[1] = IntRect(65, 403, 34, 25);
      mummyTorsoWalkFrames[2] = IntRect(114, 403, 30, 23);
      mummyTorsoWalkFrames[3] = IntRect(159, 403, 29, 21);
      mummyTorsoWalkFrames[4] = IntRect(203, 403, 28, 20);
      mummyTorsoWalkFrames[5] = IntRect(246, 403, 30, 23);
      mummyTorsoWalkFrames[6] = IntRect(291, 403, 33, 25);
      mummyTorsoWalkFrames[7] = IntRect(339, 403, 35, 26);
      mummyTorsoWalkFrames[8] = IntRect(389, 403, 34, 26);
      mummyTorsoWalkFrames[9] = IntRect(438, 403, 31, 26);
      mummyTorsoWalkFrames[10] = IntRect(484, 403, 28, 26);
      mummyTorsoWalkFrames[11] = IntRect(527, 403, 30, 26);

      mummyShootFrontFrames[0] = IntRect(15, 1351, 55, 26);
      mummyShootFrontFrames[1] = IntRect(85, 1351, 56, 25);
      mummyShootFrontFrames[2] = IntRect(156, 1351, 34, 24);
      mummyShootFrontFrames[3] = IntRect(231, 1351, 33, 25);
      mummyShootFrontFrames[4] = IntRect(279, 1351, 30, 27);
      mummyShootFrontFrames[5] = IntRect(324, 1351, 27, 27);
      mummyShootFrontFrames[6] = IntRect(366, 1351, 35, 26);
      mummyShootFrontFrames[7] = IntRect(416, 1351, 34, 25);

      mummyShootUpFrames[0] = IntRect(15, 1411, 23, 62);
      mummyShootUpFrames[1] = IntRect(53, 1411, 22, 62);
      mummyShootUpFrames[2] = IntRect(90, 1411, 23, 63);
      mummyShootUpFrames[3] = IntRect(128, 1411, 23, 33);
      mummyShootUpFrames[4] = IntRect(166, 1411, 23, 34);
      mummyShootUpFrames[5] = IntRect(204, 1411, 23, 38);
      mummyShootUpFrames[6] = IntRect(244, 1411, 22, 40);
      mummyShootUpFrames[7] = IntRect(281, 1411, 24, 43);

      // Mummy jump leg frames (from mummySheet, Y=280)
      mummyJumpLegFrames[0] = IntRect(15, 280, 13, 29);
      mummyJumpLegFrames[1] = IntRect(43, 280, 11, 29);
      mummyJumpLegFrames[2] = IntRect(69, 280, 11, 29);

      // Mummy conversion frames (from mummySheet, Y=33)
      mummyConvertFrames[0] = IntRect(15, 33, 29, 39);
      mummyConvertFrames[1] = IntRect(59, 33, 29, 41);
      mummyConvertFrames[2] = IntRect(103, 33, 29, 39);
      mummyConvertFrames[3] = IntRect(147, 33, 29, 41);

      mummyCrouchSitFrames[0] = IntRect(928, 4600, 40, 40);
      mummyCrouchSitFrames[1] = IntRect(977, 4600, 38, 40);
      mummyCrouchSitFrames[2] = IntRect(1024, 4604, 36, 36);
      mummyCrouchSitFrames[3] = IntRect(1068, 4607, 39, 33);
      mummyCrouchSitFrames[4] = IntRect(1110, 4616, 46, 24);
      mummyCrouchSitFrames[5] = IntRect(1163, 4620, 48, 20);

      mummyCrouchIdleFrames[0] = IntRect(1230, 4616, 46, 24);
      mummyCrouchIdleFrames[1] = IntRect(1284, 4616, 45, 24);
      mummyCrouchIdleFrames[2] = IntRect(1334, 4616, 45, 24);

      mummyCrouchMoveFrames[0] = IntRect(927, 4656, 46, 24);
      mummyCrouchMoveFrames[1] = IntRect(980, 4656, 47, 24);
      mummyCrouchMoveFrames[2] = IntRect(1035, 4656, 47, 24);
      mummyCrouchMoveFrames[3] = IntRect(1090, 4657, 47, 23);
      mummyCrouchMoveFrames[4] = IntRect(1143, 4656, 47, 24);
      mummyCrouchMoveFrames[5] = IntRect(1197, 4656, 46, 24);
      mummyCrouchMoveFrames[6] = IntRect(1250, 4656, 45, 24);
      mummyCrouchMoveFrames[7] = IntRect(1303, 4656, 45, 24);

      mummyDieFrames[0] = IntRect(930, 4997, 43, 47);
      mummyDieFrames[1] = IntRect(991, 4998, 37, 47);
      mummyDieFrames[2] = IntRect(1046, 4991, 45, 62);
      mummyDieFrames[3] = IntRect(1100, 4997, 54, 58);
      mummyDieFrames[4] = IntRect(1163, 5001, 50, 51);
      mummyDieFrames[5] = IntRect(1225, 4999, 53, 51);
      mummyDieFrames[6] = IntRect(1291, 4991, 57, 60);
      mummyDieFrames[7] = IntRect(1355, 4993, 58, 58);
      mummyDieFrames[8] = IntRect(1421, 4996, 57, 54);
      mummyDieFrames[9] = IntRect(1488, 4997, 54, 51);
      mummyDieFrames[10] = IntRect(1551, 4997, 55, 55);
      mummyDieFrames[11] = IntRect(1615, 4993, 57, 59);
      mummyDieFrames[12] = IntRect(1680, 4996, 54, 57);
      mummyDieFrames[13] = IntRect(1748, 4996, 58, 57);
      mummyDieFrames[14] = IntRect(946, 5073, 56, 53);
      mummyDieFrames[15] = IntRect(1014, 5073, 60, 53);
      mummyDieFrames[16] = IntRect(1086, 5076, 60, 50);
      mummyDieFrames[17] = IntRect(1157, 5077, 57, 49);
      mummyDieFrames[18] = IntRect(1225, 5074, 50, 53);
      mummyDieFrames[19] = IntRect(1295, 5075, 53, 52);
      mummyDieFrames[20] = IntRect(1368, 5073, 54, 54);
      mummyDieFrames[21] = IntRect(1439, 5075, 55, 52);
      mummyDieFrames[22] = IntRect(1504, 5103, 48, 24);
      mummyDieFrames[23] = IntRect(1571, 5097, 47, 30);
      mummyDieFrames[24] = IntRect(1640, 5107, 47, 20);
    } else {
      cout << "ERROR: Failed to load " << mummySheetPath << endl;
    }
  }

  void updateTransformationAnimation(float dt) {
    const char *state = getTransStateName();
    bool isZombie = areEqual(state, "Undead");
    bool isMummy = areEqual(state, "Mummy");
    bool isFat = areEqual(state, "Fat");

    if (!isZombie && !isMummy && !isFat) {
      isConverting = false;
      return;
    }

    bool isMoving = (abs(velocityX) > 0.1f);
    bool isFiring = (fireAnimTimer > 0);
    bool isCrouching = getIsCrouching();
    bool isFastRunning = isMoving && abs(velocityX) > baseMaxSpeed * 1.05f;

    // Handle Conversion Sequence First
    if (isConverting) {
      conversionAnimTimer += dt;
      if (conversionAnimTimer >= animDelay) {
        conversionAnimTimer = 0;
        conversionFrame++;
      }
      // mummyConvertFrames now initialized (4 frames from mummySheet Y=33)
      // zombieConvertFrames now initialized (16 frames from zombieSheet Y=186)
      int maxConvFrames = isZombie ? 16 : (isMummy ? 4 : 18);
      IntRect *convFrames =
          isZombie ? zombieConvertFrames
                   : (isMummy ? mummyConvertFrames : motuConvertFrames);

      if (conversionFrame >= maxConvFrames) {
        isConverting = false; // Done converting
      } else {
        IntRect rect = convFrames[conversionFrame];
        if (isFacingLeft) {
          rect.left += rect.width;
          rect.width = -rect.width;
        }
        // zombieConvertFrames → zombieSheet (Y~186)
        // mummyConvertFrames → mummySheet (Y~33)
        // motuConvertFrames → mainSheet (Y~7423)
        if (isZombie) {
          torsoSprite.setTexture(zombieSheet);
        } else if (isMummy) {
          torsoSprite.setTexture(mummySheet);
        } else {
          torsoSprite.setTexture(fatSheet.getSize().x > 0 ? fatSheet : mainSheet);
        }
        torsoSprite.setTextureRect(rect);
        legSprite.setTextureRect(
            IntRect(0, 0, 0, 0)); // Hide legs during conversion
        return;
      }
    }

    // Handle Death Sequence
    if (isDying) {
      deathAnimTimer += dt;
      if (deathAnimTimer >= animDelay) {
        deathAnimTimer = 0;
        deathFrame++;
      }

      // zombieDieFrames is never initialized → skip zombie death anim
      int maxDFrames = isZombie ? 0 : (isMummy ? 25 : 13);
      IntRect *deathFrames = isMummy ? mummyDieFrames : motuDieFrames;

      if (isZombie || deathFrame >= maxDFrames) {
        isDead = false;
        isAlive = false;
        return;
      }

      IntRect dRect = deathFrames[deathFrame];
      if (isFacingLeft) {
        dRect.left += dRect.width;
        dRect.width = -dRect.width;
      }
      // mummyDieFrames → mainSheet (Y~4997), motuDieFrames → mainSheet (Y~8747)
      torsoSprite.setTexture(isFat ? (fatSheet.getSize().x > 0 ? fatSheet : mainSheet) : mainSheet);
      torsoSprite.setTextureRect(dRect);
      legSprite.setTextureRect(IntRect(0, 0, 0, 0)); // Hide legs during death
      return;
    }

    // Normal State Logic
    IntRect *tFrames = nullptr;
    IntRect *lFrames = nullptr;
    int tMax = 1, lMax = 1;

    float activeDelay = animDelay;
    if (isFiring)
      activeDelay *= 0.5f;
    animTimer += dt;
    if (animTimer >= activeDelay) {
      animTimer = 0;
      currentFrame++;
    }

    if (isZombie) {
      // zombieWalkFrames → zombieSheet (Y~396)
      // zombieJumpBodyFrames → zombieSheet (Y~635)
      // All other zombie frames → mainSheet (Y 5700+)
      bool useZombieSheet = false;
      bool useLegs = false;
      IntRect *lFrames = nullptr;
      int lMax = 1;

      if (!isOnGround) {
        if (isFiring) {
          tFrames = isLookingUp ? zombieJumpHandUpFrames
                                : zombieJumpHandStraightFrames;
          tMax = 16;
        } else {
          tFrames = zombieJumpBodyFrames;
          tMax = 7;
          useZombieSheet = true;
        }
      } else if (isFastRunning) {
        if (isFiring) {
          tFrames = isLookingUp ? zombieFastHandUpFrames
                                : zombieFastHandStraightFrames;
          tMax = 24;
          lFrames = runLegFrames;
          lMax = numRunFrames;
          useLegs = true;
        } else {
          tFrames = zombieFastBodyFrames;
          tMax = 24;
        }
      } else if (isMoving) {
        if (isFiring) {
          if (isLookingUp) {
            tFrames = zombieHandUpFireFrames;
            tMax = 10;
          } else {
            tFrames = zombieHandFireFrames;
            tMax = 12;
          }
          lFrames = runLegFrames;
          lMax = numRunFrames;
          useLegs = true;
        } else {
          tFrames = zombieWalkFrames;
          useZombieSheet = true;
          tMax = 12;
        }
      } else {
        if (isFiring) {
          // Idle + firing: use full-body zombie shoot from zombieSheet
          tFrames = zombieShootFrames;
          tMax = 23;
          useZombieSheet = true;
        } else {
          tFrames = zombieWalkFrames;
          tMax = 12;
          useZombieSheet = true;
        }
      }

      IntRect tRect = tFrames[currentFrame % tMax];
      if (isFacingLeft) {
        tRect.left += tRect.width;
        tRect.width = -tRect.width;
      }
      torsoSprite.setTexture(useZombieSheet ? zombieSheet : marcoSheet);
      torsoSprite.setTextureRect(tRect);

      if (useLegs && lFrames) {
        IntRect lRect = lFrames[currentFrame % lMax];
        if (isFacingLeft) {
          lRect.left += lRect.width;
          lRect.width = -lRect.width;
        }
        legSprite.setTexture(mainSheet);
        legSprite.setTextureRect(lRect);
      } else {
        legSprite.setTextureRect(IntRect(0, 0, 0, 0));
      }

    } else if (isMummy) {
      // mummySheet frames: TorsoIdle, TorsoWalk, LegIdle, LegWalk, ShootFront, ShootUp (Y<1500)
      // mainSheet frames: JumpLeg, CrouchSit, CrouchIdle, CrouchMove, Die (Y>4000)
      // mummyFireCrouchFrames & mummyFireDownFrames are NEVER initialized → fallback
      bool torsoFromMummy = true;
      bool legFromMummy = true;

      if (!isOnGround) {
        tFrames = mummyTorsoIdleFrames; tMax = 1;   // mummySheet
        lFrames = mummyJumpLegFrames;   lMax = 3;   // mummySheet (Y=280)
      } else if (isCrouching) {
        if (isFiring) {
          tFrames = mummyShootFrontFrames; tMax = 8; // mummySheet (fallback)
        } else if (isMoving) {
          tFrames = mummyCrouchMoveFrames; tMax = 8; // mainSheet
          torsoFromMummy = false;
        } else {
          tFrames = mummyCrouchIdleFrames; tMax = 3; // mainSheet
          torsoFromMummy = false;
        }
        lFrames = mummyLegIdleFrames; lMax = 1;      // mummySheet
      } else if (isFiring) {
        // mummyFireDownFrames not initialized → use mummyShootFrontFrames
        tFrames = isLookingUp ? mummyShootUpFrames : mummyShootFrontFrames;
        tMax = 8;                                    // mummySheet
        lFrames = isMoving ? mummyLegWalkFrames : mummyLegIdleFrames;
        lMax = isMoving ? 12 : 1;                    // mummySheet
      } else {
        tFrames = isMoving ? mummyTorsoWalkFrames : mummyTorsoIdleFrames;
        tMax = isMoving ? 12 : 6;                    // mummySheet
        lFrames = isMoving ? mummyLegWalkFrames : mummyLegIdleFrames;
        lMax = isMoving ? 12 : 1;                    // mummySheet
      }

      // Safety: ensure lFrames is never null
      if (!lFrames) {
        lFrames = mummyLegIdleFrames;
        lMax = 1;
      }

      IntRect tRect = tFrames[currentFrame % tMax];
      IntRect lRect = lFrames[currentFrame % lMax];
      if (isFacingLeft) {
        tRect.left += tRect.width; tRect.width = -tRect.width;
        lRect.left += lRect.width; lRect.width = -lRect.width;
      }

      torsoSprite.setTexture(torsoFromMummy ? mummySheet : mainSheet);
      legSprite.setTexture(legFromMummy ? mummySheet : mainSheet);
      torsoSprite.setTextureRect(tRect);
      legSprite.setTextureRect(isCrouching ? IntRect(0, 0, 0, 0) : lRect);

    } else if (isFat) {
      if (hitAnimTimer > 0) {
        tFrames = motuHitFrames;
        tMax = 11;
        lFrames = fatLegIdleFrames;
        lMax = 1;
      } else if (isCrouching) {
        tFrames =
            isFiring ? motuFireCrouchFrames
                     : (isMoving ? motuCrouchMoveFrames : motuCrouchIdleFrames);
        tMax = isFiring ? 4 : (isMoving ? 4 : 6);
        lFrames = fatLegIdleFrames;
        lMax = 1;
      } else if (isFiring) {
        tFrames = isLookingUp ? motuFireUp90Frames
                              : (aimAngle < -0.05f ? motuFireDownFrames
                                                   : motuFireStraightFrames);
        tMax = isLookingUp ? 4 : (aimAngle < -0.05f ? 4 : 8);
        lFrames = isMoving ? fatLegWalkFrames : fatLegIdleFrames;
        lMax = isMoving ? 12 : 1;
      } else {
        tFrames = isMoving ? fatTorsoWalkFrames : fatTorsoIdleFrames;
        tMax = isMoving ? 12 : 4; // motuWalk is 12 frames
        lFrames = isMoving ? fatLegWalkFrames : fatLegIdleFrames;
        lMax = isMoving ? 12 : 1;
      }

      IntRect tRect = tFrames[currentFrame % tMax];
      IntRect lRect = lFrames[currentFrame % lMax];
      if (isFacingLeft) {
        tRect.left += tRect.width;
        tRect.width = -tRect.width;
        lRect.left += lRect.width;
        lRect.width = -lRect.width;
      }
      torsoSprite.setTexture(fatSheet.getSize().x > 0 ? fatSheet : mainSheet);
      legSprite.setTexture(fatSheet.getSize().x > 0 ? fatSheet : mainSheet);
      torsoSprite.setTextureRect(tRect);
      legSprite.setTextureRect(isCrouching ? IntRect(0, 0, 0, 0) : lRect);
    }
  }

  PlayerSoldier(double posX, double posY)
      : Soldier(posX, posY, 10), isActivePlayer(false), pistolDamage(3),
        knifeDamage(2), pistolFireRate(4.0), meleeRange(0.0),
        meleeCooldown(0.5), grenadeCount(10), grenadeMax(10), lives(2),
        currentWeapon(nullptr), inventorySize(0), currentWeaponIndex(0),
        scores(0), saturation(10.0), isDead(false), damageState(0),
        damageStateTimer(0), specialActive(false), specialDuration(0),
        isOccupied(false), tWasPressed(false), isLookingUp(false),
        fireAnimTimer(0), hitAnimTimer(0), isDying(false), deathType(0),
        deathFrame(0), deathAnimTimer(0), isConverting(false),
        conversionFrame(0), conversionAnimTimer(0), scaleFactor(3),
        frameWidth(28), frameHeight(42), legOffsetY(0), legOffsetX(0),
        numIdleFrames(1), numRunFrames(1), numJumpFrames(1),
        numJumpIntroFrames(0), numShootFrames(1), numShootUpFrames(1),
        numKnifeFrames(1), numShootHMGFrames(1), numFastRunFrames(6),
        numNeatFrames(16), numParachuteFlyingFrames(5),
        numParachuteFallingFrames(17), isNeatCalling(false),
        neatCallTimer(0.0f), baseMaxSpeed(5), oWasPressed(false),
        gWasPressed(false), fWasPressed(false), iWasPressed(false),
        kWasPressed(false), nWasPressed(false), pWasPressed(false),
        isInvincible(false), lastTorsoSet(nullptr), lastLegSet(nullptr),
        aimAngle(0), qWasPressed(false), wWasPressed(false),
        isParachuting(false), parachuteState(0), parachuteLandX(0),
        parachuteLandY(0), parachuteAnimTimer(0), parachuteFrame(0),
        wasJumpingFromRun(false), isSwimming(false), swimAngle(0) {
    for (int i = 0; i < 10; i++)
      inventory[i] = nullptr;

    inventory[0] = new Pistol();
    inventory[1] = new HeavyMachineGun();
    inventory[2] = new RocketLauncher();
    inventory[3] = new LaserGun();
    inventory[4] = new FlameShot();
    inventory[5] = new Knife();
    inventorySize = 6;
    neatAgent = new NeatAgent();
    neatAgent->generateDefaultBrain(7, 4); // 7 inputs, 4 outputs

    currentWeaponIndex = 0;
    currentWeapon = inventory[currentWeaponIndex];
    grenadeWeapon = new HandGrenade();

    if (parachuteSheet.loadFromFile("Sprites/Marco Rossi 2.png")) {
      parachuteSprite.setTexture(parachuteSheet);
    }
    parachuteSprite.setScale(scaleFactor, scaleFactor);

    parachuteFlyingFrames[0] = IntRect(919, 8535, 52, 44);
    parachuteFlyingFrames[1] = IntRect(976, 8535, 51, 44);
    parachuteFlyingFrames[2] = IntRect(1032, 8535, 52, 44);
    parachuteFlyingFrames[3] = IntRect(1089, 8535, 52, 44);
    parachuteFlyingFrames[4] = IntRect(1146, 8536, 52, 43);

    parachuteFallingFrames[0] = IntRect(920, 8583, 54, 44);
    parachuteFallingFrames[1] = IntRect(979, 8583, 54, 45);
    parachuteFallingFrames[2] = IntRect(1038, 8584, 56, 44);
    parachuteFallingFrames[3] = IntRect(1099, 8586, 55, 48);
    parachuteFallingFrames[4] = IntRect(1159, 8592, 53, 43);
    parachuteFallingFrames[5] = IntRect(1217, 8599, 54, 40);
    parachuteFallingFrames[6] = IntRect(1276, 8607, 56, 33);
    parachuteFallingFrames[7] = IntRect(1337, 8616, 62, 26);
    parachuteFallingFrames[8] = IntRect(1404, 8616, 51, 19);
    parachuteFallingFrames[9] = IntRect(1460, 8609, 44, 25);
    parachuteFallingFrames[10] = IntRect(1510, 8609, 35, 25);
    parachuteFallingFrames[11] = IntRect(1550, 8615, 33, 16);
    parachuteFallingFrames[12] = IntRect(1588, 8611, 22, 18);
    parachuteFallingFrames[13] = IntRect(1615, 8617, 25, 11);
    parachuteFallingFrames[14] = IntRect(1645, 8616, 20, 12);
    parachuteFallingFrames[15] = IntRect(1670, 8615, 18, 9);
    parachuteFallingFrames[16] = IntRect(1693, 8616, 16, 8);

    spawnX = posX;
    spawnY = posY;
    companionTarget = nullptr;
    currentVehicle = nullptr;
  }

  void resolveCollision(Projectile *p, float dt) override;
  void resolveCollision(Collectible *c, float dt) override;

  virtual ~PlayerSoldier() { delete neatAgent; }

  void setIsActivePlayer(bool v) {
    isActivePlayer = v;
    if (v) {
      upWasPressed = Keyboard::isKeyPressed(Keyboard::X);
      companionTarget = nullptr;
    }
  }
  void setCompanionTarget(PlayerSoldier *p) { companionTarget = p; }
  bool getIsActivePlayer() const { return isActivePlayer; }

  bool isPlayer() const override { return true; }
  int getFaction() const override { return FACTION_PLAYER; }
  PlayerSoldier *getAsPlayerSoldier() override { return this; }

  float getWidth() const override { return frameWidth * scaleFactor; }
  float getHeight() const override {
    float legH = abs(idleLegFrames[0].height) * scaleFactor;
    float fullH = legOffsetY + legH;
    return getIsCrouching() ? fullH * 0.5f : fullH;
  }

  void setCurrentVehicle(Vehicle *v) { currentVehicle = v; }
  Vehicle *getCurrentVehicle() const { return currentVehicle; }

  // getters / Setters
  void setPistolDamage(float v) { pistolDamage = v; }
  float getPistolDamage() const { return pistolDamage; }
  void setKnifeDamage(float v) { knifeDamage = v; }
  float getKnifeDamage() const { return knifeDamage; }
  void setLives(int v) { lives = v; }
  int getLives() const { return lives; }
  void setGrenadeCount(int v) { grenadeCount = v; }
  int getGrenadeCount() const { return grenadeCount; }
  void setScores(int v) { scores = v; }
  int getScores() const { return scores; }
  double getSaturation() const { return saturation; }
  bool getIsDead() const { return isDead; }
  void setIsDead(bool v) { isDead = v; }
  int getDamageState() const { return damageState; }
  double getPistolFireRate() const { return pistolFireRate; }
  bool getSpecialActive() const { return specialActive; }

  void addSaturation(double delta) {
    saturation += delta;
    if (saturation > 10.0)
      saturation = 10.0;
  }

  void replenishAmmo(int quantity) {
    if (!currentWeapon)
      return;
    FireArms *fa = dynamic_cast<FireArms *>(currentWeapon);
    if (fa)
      fa->setBullets(fa->getBullets() + quantity);
  }

  bool getIsAlive() const override { return true; }

  // feature 1: Damage State System
  // each hit advances state: HEALTHY→INJURED(1s)→CRITICAL(1s)→DEAD.
  // overrides DamagableEntity::takeDamage so ALL damage paths go through it.
  void takeDamage(int val) override {
    if (val <= 0 || isInvincible)
      return;

    int hits = 1;
    if (val >= 40)
      hits = 3;
    else if (val >= 10)
      hits = 2;

    bool lethal = false;
    for (int i = 0; i < hits; i++) {
      if (damageOverlay.onHit())
        lethal = true;
    }

    if (lethal) {
      die();
    } else {
      hitAnimTimer = 0.5f; // Play hit animation for 0.5s
    }
  }

  DamageOverlay &getDamageOverlay() { return damageOverlay; }

  void die() override {
    if (!isDying) {
      isDying = true;
      deathFrame = 0;
      deathAnimTimer = 0;
      velocityX = 0;

      // Play character-specific death scream
      if (isMarco()) {
        SoundManager::playSound("sound/marco_die.ogg");
      } else if (isTarma()) {
        SoundManager::playSound("sound/tarma_die.ogg");
      } else if (isEri()) {
        SoundManager::playSound("sound/eri_die.ogg");
      } else if (isFio()) {
        SoundManager::playSound("sound/fio_die.ogg");
      }
    }
  }

  void updateAim() {
    const float STEP = (3.14159f / 2) / 30; // 90° over 30 frames
    bool upNow = Keyboard::isKeyPressed(Keyboard::Up) ||
                 Keyboard::isKeyPressed(Keyboard::Q);
    bool downNow = Keyboard::isKeyPressed(Keyboard::Down) ||
                   Keyboard::isKeyPressed(Keyboard::W);
    bool moveNow = Keyboard::isKeyPressed(Keyboard::Left) ||
                   Keyboard::isKeyPressed(Keyboard::Right);

    if (upNow) {
      aimAngle += STEP;
      if (aimAngle > 3.14159f / 2)
        aimAngle = 3.14159f / 2;
    } else if (downNow) {
      aimAngle -= STEP;
      if (aimAngle < 0)
        aimAngle = 0;
    } else if (moveNow) {

      aimAngle -= STEP * 2; // faster reset
      if (aimAngle < 0)
        aimAngle = 0;
    }

    isLookingUp = (aimAngle > 0.05f);
  }

  virtual void shoot() {
    if (!currentWeapon)
      return;

    if (currentWeapon->isMelee()) {
      if (currentWeapon->canFire()) {
        currentWeapon->attackMelee(posX, posY, !isFacingLeft);
        if (fireAnimTimer <= 0) {
          currentFrame = 0;
          animTimer = 0;
        }
        fireAnimTimer = numKnifeFrames * (animDelay * 0.5f);
      }
      return;
    }

    double baseAngle = isFacingLeft ? 3.14159265 : 0.0;
    double fireAngle = isFacingLeft ? (3.14159265 + aimAngle) : -aimAngle;
    double muzzleX = posX + (isFacingLeft ? 0.0 : getWidth());
    double muzzleY = posY + getHeight() * 0.5 - aimAngle * getHeight() * 0.2;

    pendingShot = currentWeapon->fire(muzzleX, muzzleY, fireAngle, true);

    if (pendingShot) {
      if (fireAnimTimer <= 0) {
        currentFrame = 0;
        animTimer = 0;
      }
      float factor = isMarco() ? (0.5f / 1.5f) : 0.5f;
      fireAnimTimer = numShootFrames * (animDelay * factor);
    }
  }

  // grenade throw (F key)

  virtual void throwGrenade() {
    if (grenadeCount <= 0 || !grenadeWeapon)
      return;

    double dir = isFacingLeft ? -1.0 : 1.0;
    double angle =
        isFacingLeft ? 3.14159265 - 0.785398 : -0.785398; // 45 deg up

    double muzzleX = posX + (dir < 0 ? 0.0 : getWidth());
    double muzzleY = posY + getHeight() * 0.25;

    pendingShot = grenadeWeapon->fire(muzzleX, muzzleY, angle, true);
    if (pendingShot)
      grenadeCount--;
  }

  virtual void meleeAttack() {}

  virtual void pickupWeapon(Weapon *w) {
    for (int i = 0; i < 10; i++) {
      if (inventory[i] == nullptr) {
        inventory[i] = w;
        if (inventorySize < 10)
          inventorySize++;
        currentWeapon = w;
        SoundManager::playSound("sound/enemy_chaser_voice.ogg");
        break;
      }
    }
  }

  void switchWeapon(int slot) {
    if (slot >= 0 && slot < inventorySize && inventory[slot])
      currentWeapon = inventory[slot];
  }

  void jump() override {
    if (jumpCount < maxJumps) {
      velocityY = -jump_strength;
      jumpCount++;
      SoundManager::playSound("sound/jump.ogg");
    }
  }

  void onGroundCollision(float groundY) {
    if (!isOnGround) {
      SoundManager::playSound("sound/land.ogg");
    }
    DamagableEntity::onGroundCollision(groundY);
  }

  void respawn() {
    if (lives > 0) {
      lives--;
      setHp(10);
      posX = spawnX;
      posY = spawnY;
      velocityX = 0;
      velocityY = 0;
      isDead = false;
      isAlive = true;
    }
  }

  // character-specific subclasses implement these
  virtual void activateSpecial() = 0;
  virtual void applyCharacterBuffs() = 0;
  virtual void loadAnimation(int type) { (void)type; }
  virtual const char *getCharacterName() const = 0;
  virtual float getSpecialFireRateMultiplier() const { return 1; }

  // character identity hooks (for FusionCompanion stat averaging)
  virtual bool isMarco() const { return false; }
  virtual bool isTarma() const { return false; }
  virtual bool isEri() const { return false; }
  virtual bool isFio() const { return false; }

  // vehicle buffs (Step 6)
  virtual float getVehicleFireRateMultiplier() const { return 1; }
  virtual float getVehicleDurabilityMultiplier() const { return 1; }

  // master update
  // entity::update implementation
  void update(Level *level, float dt) override { updateLogic(level, dt); }

  // refactored update logic to split physics from rendering
  void updateLogic(Level *level, float dt);

  // draw
  virtual void draw(RenderWindow &window) override {
    if (isOccupied)
      return;

    // Transformation animation and normal animation are now updated in
    // updateLogic()
    updateSpritePositions();

    TransformationState *ts = getTransState();
    bool isZombie = (ts && ts->getID() == TRANS_STATE_UNDEAD);

    // only draw legs if not a zombie (or if zombie has active leg frames)
    if (!isZombie || legSprite.getTextureRect() != IntRect(0, 0, 0, 0)) {
      window.draw(legSprite);
    }
    window.draw(torsoSprite);

    if (isParachuting && isTarma()) {
      window.draw(parachuteSprite);
    }
  }

  PlayerSoldier &operator+=(int val) {
    scores += val;
    return *this;
  }

  void onCollideWith(Entity *other, float dt) override {
    other->resolveCollision(this, dt);
  }
};
#include "../Vehicle System/Vehicle.h"

using namespace sf;
using namespace std;

inline void PlayerSoldier::updateLogic(Level *level, float dt) {
  if (isDead) {
    respawn();
    return;
  }

  if (isDying) {
    applyPhysics();
    tileCollisionImpl(level);

    // Death animation updates
    deathAnimTimer += dt;
    if (deathAnimTimer >= animDelay) {
      deathAnimTimer = 0;
      deathFrame++;
    }

    int maxDeathFrames = 26; // Normal death length for Tarma
    IntRect *frames = dieNormalFrames;
    if (deathType == 1) {
      frames = dieFireFrames;
      maxDeathFrames = 25;
    } else if (deathType == 2) {
      frames = dieAlienFrames;
      maxDeathFrames = 35;
    } else if (deathType == 3) {
      frames = dieAquaticFrames;
      maxDeathFrames = 20;
    }

    if (deathFrame >= maxDeathFrames) {
      isDead = true; // Trigger respawn on next frame
      isDying = false;
      return;
    }

    IntRect tRect = frames[deathFrame];
    if (isFacingLeft) {
      tRect.left += tRect.width;
      tRect.width = -tRect.width;
    }
    torsoSprite.setTexture(mainSheet);
    torsoSprite.setTextureRect(tRect);
    // Hide legs during death
    legSprite.setTextureRect(IntRect(0, 0, 0, 0));
    updateSpritePositions();
    return;
  }

  // companion and target assignment is now handled by EntityManager::update()

  if (isActivePlayer) {
    if (getIsOccupied() && currentVehicle) {
      bool tNow = Keyboard::isKeyPressed(Keyboard::T);
      if (tNow && !tWasPressed) {
        currentVehicle->setDriver(nullptr);
        currentVehicle = nullptr;
        setIsOccupied(false);
        posY -= (getHeight() + 10);
      }
      tWasPressed = tNow;
    }

    bool gNow = Keyboard::isKeyPressed(Keyboard::G);
    if (gNow && !gWasPressed && !specialActive)
      activateSpecial();
    gWasPressed = gNow;

    if (!getIsOccupied()) {
      if (!Keyboard::isKeyPressed(Keyboard::T)) {
        tWasPressed = false;
      }
      handleInput();
      applyPhysics();
      tileCollisionImpl(level);

      // Fast run logic
      if (isFlying) {
        max_speed = baseMaxSpeed * 4.5f;
      } else if (Keyboard::isKeyPressed(Keyboard::LShift)) {
        max_speed = baseMaxSpeed * 1.6f;
      } else {
        max_speed = baseMaxSpeed;
      }

      // Swimming logic: true if in aquatic biome (1)
      isSwimming = (level->getBiomeAt(posX) == 1);
      if (isSwimming) {
        // Optional: update swimAngle based on up/down keys
        if (Keyboard::isKeyPressed(Keyboard::Up) ||
            Keyboard::isKeyPressed(Keyboard::Q)) {
          swimAngle = 2.0f; // More angle up
        } else if (Keyboard::isKeyPressed(Keyboard::Down) ||
                   Keyboard::isKeyPressed(Keyboard::W)) {
          swimAngle = -2.0f; // More angle down
        } else {
          swimAngle = 0.0f; // Straight
        }
      }

      bool oNow = Keyboard::isKeyPressed(Keyboard::O);
      if (oNow && !oWasPressed) {
        currentWeaponIndex = (currentWeaponIndex + 1) % inventorySize;
        currentWeapon = inventory[currentWeaponIndex];
      }
      oWasPressed = oNow;

      // Update animation state in update cycle instead of draw
      TransformationState *ts = getTransState();
      int tsID = ts ? ts->getID() : TRANS_STATE_NORMAL;
      bool isZombie = (tsID == TRANS_STATE_UNDEAD);
      bool isMummy = (tsID == TRANS_STATE_MUMMY);
      bool isFat = (tsID == TRANS_STATE_FAT);

      if (isZombie || isMummy || isFat) {
        updateTransformationAnimation(dt);
      } else {
        updateAnimation(dt);
      }

      if (currentWeapon) {
        float weaponDt = dt;
        weaponDt *= getSpecialFireRateMultiplier();
        currentWeapon->tickCooldown(weaponDt);
      }
      if (fireAnimTimer > 0)
        fireAnimTimer -= dt;
      if (hitAnimTimer > 0)
        hitAnimTimer -= dt;

      updateAim();

      bool pNow = Keyboard::isKeyPressed(Keyboard::P);
      if (pNow && !pWasPressed) {
        isFlying = !isFlying;
        if (isFlying) {
          velocityX = 0;
          velocityY = 0;
        }
      }
      pWasPressed = pNow;

      if (isFlying) {
        float flySpeed = max_speed * 4.5f;
        if (Keyboard::isKeyPressed(Keyboard::Up) ||
            Keyboard::isKeyPressed(Keyboard::Q)) {
          velocityY = -flySpeed;
        } else if (Keyboard::isKeyPressed(Keyboard::Down) ||
                   Keyboard::isKeyPressed(Keyboard::W)) {
          velocityY = flySpeed;
        } else {
          velocityY = 0;
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) {
          velocityX = -flySpeed;
          isFacingLeft = true;
        } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
          velocityX = flySpeed;
          isFacingLeft = false;
        } else {
          velocityX = 0;
        }
      }

      if (Keyboard::isKeyPressed(Keyboard::Space))
        shoot();

      bool fNow = Keyboard::isKeyPressed(Keyboard::F);
      if (fNow && !fWasPressed)
        throwGrenade();
      fWasPressed = fNow;

    } else {
      if (currentVehicle) {
        posX = currentVehicle->getPosX();
        posY = currentVehicle->getPosY();
        isFacingLeft = currentVehicle->getIsFacingLeft();
      }
    }
  } else {
    bool aLeft = false, aRight = false, aJump = false, aShoot = false;
    float currentRunSpeed = baseMaxSpeed;

    if (companionTarget && neatAgent && !isDead) {

      double inputs[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

      // Inputs 0-2: Leader player position and distance
      float dxLeader = companionTarget->getPosX() - posX;
      float dyLeader = companionTarget->getPosY() - posY;
      inputs[0] = dxLeader;
      inputs[1] = dyLeader;
      inputs[2] = abs(dxLeader);

      // Inputs 3-5: Nearest Enemy position and distance
      DamagableEntity *enemyTarget = getTarget();
      if (enemyTarget) {
        float dxEnemy = enemyTarget->getPosX() - posX;
        float dyEnemy = enemyTarget->getPosY() - posY;
        inputs[3] = dxEnemy;
        inputs[4] = dyEnemy;
        inputs[5] = abs(dxEnemy);
      }

      // Input 6: Player physics state
      inputs[6] = isOnGround ? 1.0 : 0.0;

      // Ask the NEAT Neural Network what buttons to press
      neatAgent->thinkAndAct(inputs, 7, aJump, aShoot, aLeft, aRight);

      if (abs(dxLeader) > 300)
        currentRunSpeed = baseMaxSpeed * 1.5f;
    }

    if (aJump)
      jump();
    if (aShoot)
      shoot();

    if (aLeft) {
      velocityX -= acceleration;
      if (velocityX < -currentRunSpeed)
        velocityX = -currentRunSpeed;
      isFacingLeft = true;
    } else if (aRight) {
      velocityX += acceleration;
      if (velocityX > currentRunSpeed)
        velocityX = currentRunSpeed;
      isFacingLeft = false;
    } else {
      velocityX = 0;
    }

    applyPhysics();
    tileCollisionImpl(level);

    if (currentWeapon)
      currentWeapon->tickCooldown(dt);
    if (fireAnimTimer > 0)
      fireAnimTimer -= dt;

    // Update animation state for non-active players/neat agents to prevent full spritesheet previewing
    TransformationState *ts = getTransState();
    int tsID = ts ? ts->getID() : TRANS_STATE_NORMAL;
    bool isZombie = (tsID == TRANS_STATE_UNDEAD);
    bool isMummy = (tsID == TRANS_STATE_MUMMY);
    bool isFat = (tsID == TRANS_STATE_FAT);

    if (isZombie || isMummy || isFat) {
      updateTransformationAnimation(dt);
    } else {
      updateAnimation(dt);
    }
  }

  // N key: play NEAT call animation (Section 2.8/2.9), do NOT transform to fat
  bool nNow = Keyboard::isKeyPressed(Keyboard::N);
  if (nNow && !nWasPressed && !isNeatCalling) {
    isNeatCalling = true;
    // Duration = 16 upper frames * animDelay
    neatCallTimer = numNeatFrames * animDelay;
    currentFrame = 0;
    animTimer = 0;
  }
  nWasPressed = nNow;

  // H key: toggle Fat (Heavy) transformation
  static bool hWasPressed = false;
  bool hNow = Keyboard::isKeyPressed(Keyboard::H);
  if (hNow && !hWasPressed) {
    if (areEqual(getTransStateName(), "Fat")) {
      changeTransState(nullptr);
    } else {
      transformToFat();
    }
  }
  hWasPressed = hNow;

  if (getTransState()) {
    getTransState()->update(this, dt);
    if (getTransState()->isExpired())
      changeTransState(nullptr);
  }

  // Parachute logic — TARMA ONLY, only when actually falling (not just
  // switching chars)
  if (isTarma() && !isOnGround && velocityY > 50.0f && !isSwimming) {
    if (!isParachuting && parachuteState == 0) {
      isParachuting = true;
      parachuteState = 0;
      parachuteFrame = 0;
      parachuteAnimTimer = 0;
    }
  }
  // Reset parachute state completely when character grounded and NOT tarma
  if (!isTarma()) {
    isParachuting = false;
    parachuteState = 0;
  }
  // Reset wasJumpingFromRun when we land
  if (isOnGround) {
    wasJumpingFromRun = false;
  }

  if (isParachuting && isTarma()) {
    parachuteAnimTimer += dt;
    if (parachuteAnimTimer >= animDelay) {
      parachuteAnimTimer = 0;
      parachuteFrame++;
    }

    if (parachuteState == 0) {
      if (isOnGround) {
        parachuteState = 1;
        parachuteFrame = 0;
        parachuteLandX = posX;
        parachuteLandY = posY; // ground level
      } else {
        int fIdx = parachuteFrame % numParachuteFlyingFrames;
        parachuteSprite.setTextureRect(parachuteFlyingFrames[fIdx]);

        float pOffsetX = isFacingLeft ? -10.0f : -10.0f;
        float pOffsetY = -45.0f * scaleFactor;
        parachuteSprite.setPosition(posX + pOffsetX, posY + pOffsetY);
      }
    }

    if (parachuteState == 1) {
      if (parachuteFrame < numParachuteFallingFrames) {
        parachuteSprite.setTextureRect(parachuteFallingFrames[parachuteFrame]);
      } else {
        // Remove parachute after landing sequence completes
        isParachuting = false;
        return; // Early exit so we don't draw or manipulate it this frame
      }

      float pOffsetX = isFacingLeft ? -10.0f : -10.0f;
      float pOffsetY = -25.0f * scaleFactor;
      parachuteSprite.setPosition(parachuteLandX + pOffsetX,
                                  parachuteLandY + pOffsetY);
    }

    if (isFacingLeft) {
      IntRect r = parachuteSprite.getTextureRect();
      if (r.width > 0) {
        r.left += r.width;
        r.width = -r.width;
      }
      parachuteSprite.setTextureRect(r);
    }
  }

  if (specialActive) {
    specialDuration -= dt;
    if (specialDuration <= 0)
      specialActive = false;
  }
  damageOverlay.update(dt);
}
#include "../Collectible and Interactable System/Collectible.h"
#include "../Enemy System/Enemy.h"

inline void PlayerSoldier::resolveCollision(Projectile *p, float dt) {
  if (!p || !p->getIsAlive())
    return;
  // faction filtering guarantees only enemy-owned projectiles reach here
  DamagableEntity::resolveCollision(p, dt);
}

inline void PlayerSoldier::resolveCollision(Collectible *c, float dt) {
  c->onCollect(this);
}
