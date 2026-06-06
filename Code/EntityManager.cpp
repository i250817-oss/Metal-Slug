#include "EntityManager.h"
#include "../Character System/PlayerSoldier.h"
#include "../Character System/FusionCompanion.h"
#include "SoundManager.h"
#include "../Level and Environment System/Level.h"
#include "../Level and Environment System/camera.h"
#include "../Level and Environment System/spawnPoint.h"
#include "../ScoreCard/ScoreCard.h"

// Concrete spawner types
#include "../Collectible and Interactable System/POWPrisoner.h"
#include "../Enemy System/Aerial/Paratrooper.h"
#include "../Enemy System/Alien/AlienEnemy.h"
#include "../Enemy System/Infantry/BazookaSoldier.h"
#include "../Enemy System/Infantry/GrenadeSoldier.h"
#include "../Enemy System/Infantry/RebelSoldier.h"
#include "../Enemy System/Infantry/ShieldedSoldier.h"
#include "../Enemy System/Undead/MummyWarrior.h"
#include "../Enemy System/Undead/Zombie.h"
#include "../Enemy System/Boss/FusionBoss.h"
#include "../Enemy System/Boss/HairbusterRiberts.h"
#include "../Enemy System/Boss/IronNokana.h"
#include "../Enemy System/Boss/SeaSatan.h"
#include "../Vehicle System/Enemy Vehicles/EnemySub.h"
#include "../Vehicle System/Enemy Vehicles/FlyingTara.h"
#include "../Vehicle System/Enemy Vehicles/M15ABradley.h"
#include "../Vehicle System/Player Vehicles/AmphibiousSlug.h"
#include "../Vehicle System/Player Vehicles/MetalSlug.h"
#include "../Vehicle System/Player Vehicles/SlugFlyer.h"
#include "../Vehicle System/Player Vehicles/SlugMariner.h"

// Concrete spawner implementations
class RebelSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "rebel"; }
    Entity* make(double x, double y) const override { return new RebelSoldier(x, y); }
};
class ShieldedSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "shielded"; }
    Entity* make(double x, double y) const override { return new ShieldedSoldier(x, y); }
};
class GrenadeSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "grenade"; }
    Entity* make(double x, double y) const override { return new GrenadeSoldier(x, y); }
};
class BazookaSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "bazooka"; }
    Entity* make(double x, double y) const override { return new BazookaSoldier(x, y); }
};
class ParatrooperSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "paratrooper"; }
    Entity* make(double x, double y) const override { return new AerialParatrooper(x, y, new RebelSoldier(x, y)); }
};
class ZombieSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "zombie"; }
    Entity* make(double x, double y) const override { return new Zombie(x, y); }
};
class MummySpawner : public EnemySpawner {
public:
    const char* tag() const override { return "mummy"; }
    Entity* make(double x, double y) const override { return new MummyWarrior(x, y); }
};
class AlienSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "alien"; }
    Entity* make(double x, double y) const override { return new AlienEnemy(x, y); }
};
class POWSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "pow"; }
    Entity* make(double x, double y) const override { return new POWPrisoner(x, y); }
};
class FlyingTaraSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "flying_tara"; }
    Entity* make(double x, double y) const override { return new FlyingTara(x, y); }
};
class EnemySubSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "enemy_sub"; }
    Entity* make(double x, double y) const override { return new EnemySub(x, y); }
};
class M15ABradleySpawner : public EnemySpawner {
public:
    const char* tag() const override { return "m15a_bradley"; }
    Entity* make(double x, double y) const override { return new M15ABradley(x, y); }
};
class IronNokanaSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "iron_nokana"; }
    Entity* make(double x, double y) const override { return new IronNokana(x, y); }
};
class HairbusterSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "hairbuster"; }
    Entity* make(double x, double y) const override { return new HairbusterRiberts(x, y); }
};
class SeaSatanSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "sea_satan"; }
    Entity* make(double x, double y) const override { return new SeaSatan(x, y); }
};
class FusionBossSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "fusion_boss"; }
    Entity* make(double x, double y) const override { return new FusionBoss(x, y); }
};
class MetalSlugSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "metal_slug"; }
    Entity* make(double x, double y) const override { return new MetalSlug(x, y); }
};
class SlugFlyerSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "slug_flyer"; }
    Entity* make(double x, double y) const override { return new SlugFlyer(x, y); }
};
class SlugMarinerSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "slug_mariner"; }
    Entity* make(double x, double y) const override { return new SlugMariner(x, y); }
};
class AmphibiousSlugSpawner : public EnemySpawner {
public:
    const char* tag() const override { return "amphibious_slug"; }
    Entity* make(double x, double y) const override { return new AmphibiousSlug(x, y); }
};

EntityManager::EntityManager(int initialCapacity)
    : count(0), capacity(initialCapacity), playerCount(0),
      activePlayerIndex(0), neatAgentsActive(false),
      fusionCompanion(nullptr), spawnerCount(0),
      cachedEnemyCount(0), enemyCountDirty(false),
      allSpawnsProcessed(false) {
    instance = this;
    entities = new Entity*[capacity];
    for (int i = 0; i < capacity; i++) entities[i] = nullptr;
    for (int i = 0; i < 4; i++) players[i] = nullptr;

    spawnerRegistry[spawnerCount++] = new RebelSpawner();
    spawnerRegistry[spawnerCount++] = new ShieldedSpawner();
    spawnerRegistry[spawnerCount++] = new GrenadeSpawner();
    spawnerRegistry[spawnerCount++] = new BazookaSpawner();
    spawnerRegistry[spawnerCount++] = new ParatrooperSpawner();
    spawnerRegistry[spawnerCount++] = new ZombieSpawner();
    spawnerRegistry[spawnerCount++] = new MummySpawner();
    spawnerRegistry[spawnerCount++] = new AlienSpawner();
    spawnerRegistry[spawnerCount++] = new POWSpawner();
    spawnerRegistry[spawnerCount++] = new FlyingTaraSpawner();
    spawnerRegistry[spawnerCount++] = new EnemySubSpawner();
    spawnerRegistry[spawnerCount++] = new M15ABradleySpawner();
    spawnerRegistry[spawnerCount++] = new MetalSlugSpawner();
    spawnerRegistry[spawnerCount++] = new SlugFlyerSpawner();
    spawnerRegistry[spawnerCount++] = new SlugMarinerSpawner();
    spawnerRegistry[spawnerCount++] = new AmphibiousSlugSpawner();
    spawnerRegistry[spawnerCount++] = new IronNokanaSpawner();
    spawnerRegistry[spawnerCount++] = new HairbusterSpawner();
    spawnerRegistry[spawnerCount++] = new SeaSatanSpawner();
    spawnerRegistry[spawnerCount++] = new FusionBossSpawner();
}

EntityManager::~EntityManager() {
    for (int i = 0; i < count; i++) {
        delete entities[i];
        entities[i] = nullptr;
    }
    delete[] entities;
    for (int i = 0; i < 4; i++)
        players[i] = nullptr;
    for (int i = 0; i < spawnerCount; i++) {
        delete spawnerRegistry[i];
        spawnerRegistry[i] = nullptr;
    }
}

void EntityManager::resize() {
    int newCap = capacity * 2;
    Entity **newArr = new Entity *[newCap];
    for (int i = 0; i < count; i++)
        newArr[i] = entities[i];
    for (int i = count; i < newCap; i++)
        newArr[i] = nullptr;
    delete[] entities;
    entities = newArr;
    capacity = newCap;
}

PlayerSoldier *EntityManager::getNearestPlayer(double x, double y) {
    PlayerSoldier *nearest = nullptr;
    float minDistSq = 99999.0f * 99999.0f;
    for (int i = 0; i < playerCount; i++) {
        if (players[i] && players[i]->getIsAlive()) {
            float dx = players[i]->getPosX() - x;
            float dy = players[i]->getPosY() - y;
            float dSq = dx * dx + dy * dy;
            if (dSq < minDistSq) {
                minDistSq = dSq;
                nearest = players[i];
            }
        }
    }
    return nearest;
}

Entity *EntityManager::getNearestEnemy(double x, double y) {
    Entity *nearest = nullptr;
    float minDistSq = 99999.0f * 99999.0f;
    for (int i = 0; i < count; i++) {
        if (entities[i] && entities[i]->isEnemy() && entities[i]->getIsAlive()) {
            float dx = entities[i]->getPosX() - x;
            float dy = entities[i]->getPosY() - y;
            float dSq = dx * dx + dy * dy;
            if (dSq < minDistSq) {
                minDistSq = dSq;
                nearest = entities[i];
            }
        }
    }
    return nearest;
}

void EntityManager::addEntity(Entity *e) {
    if (!e) return;
    if (count >= capacity) resize();
    entities[count++] = e;

    // Track if the new entity is an enemy so the cache stays valid
    if (e->isEnemy() && e->getIsAlive()) {
        cachedEnemyCount++;
    }

    PlayerSoldier *ps = e->getAsPlayerSoldier();
    if (ps && playerCount < 4) {
        players[playerCount] = ps;
        players[playerCount]->setIsActivePlayer(playerCount == activePlayerIndex);
        playerCount++;
    }
}

void EntityManager::removeEntityAt(int index) {
    if (index < 0 || index >= count) return;

    Entity *e = entities[index];
    for (int i = 0; i < playerCount; i++) {
        if (players[i] == e) {
            for (int j = i; j < playerCount - 1; j++)
                players[j] = players[j + 1];
            players[playerCount - 1] = nullptr;
            playerCount--;
            break;
        }
    }

    delete e;
    for (int i = index; i < count - 1; i++)
        entities[i] = entities[i + 1];
    entities[count - 1] = nullptr;
    count--;
}

long long EntityManager::removeDeadEntities(Level *level) {
    for (int i = count - 1; i >= 0; i--) {
        if (!entities[i]->getIsAlive()) {
            // Invalidate cache when an enemy is removed
            if (entities[i]->isEnemy())
                enemyCountDirty = true;
            entities[i]->handleDeath(level);
            removeEntityAt(i);
        }
    }
    return 0;
}

void EntityManager::clearEnemies() {
    for (int i = count - 1; i >= 0; i--) {
        if (entities[i] && entities[i]->isEnemy()) {
            removeEntityAt(i);
        }
    }
}

PlayerSoldier *EntityManager::getPlayerAt(int i) const {
    return (i >= 0 && i < playerCount) ? players[i] : nullptr;
}

int EntityManager::getEnemyCount() {
    if (!enemyCountDirty)
        return cachedEnemyCount;
    // Full recount only when dirty
    int enemies = 0;
    for (int i = 0; i < count; i++)
        if (entities[i] && entities[i]->isEnemy() && entities[i]->getIsAlive())
            enemies++;
    cachedEnemyCount = enemies;
    enemyCountDirty  = false;
    return enemies;
}

PlayerSoldier *EntityManager::getActivePlayer() const {
    return (activePlayerIndex >= 0 && activePlayerIndex < playerCount)
               ? players[activePlayerIndex]
               : nullptr;
}

void EntityManager::cycleActivePlayer() {
    if (playerCount <= 1) return;
    PlayerSoldier *current = players[activePlayerIndex];
    if (current) current->setIsActivePlayer(false);

    int startIndex = activePlayerIndex;
    do {
        activePlayerIndex = (activePlayerIndex + 1) % playerCount;
    } while (players[activePlayerIndex] && !players[activePlayerIndex]->getIsAlive() &&
             activePlayerIndex != startIndex);

    PlayerSoldier *next = players[activePlayerIndex];
    
    if (next && current && !neatAgentsActive) {
        float feetY = current->getPosY() + current->getHeight();
        next->setPosX(current->getPosX());
        next->setPosY(feetY - next->getHeight());
        next->setVelocityX(current->getVelocityX());
        next->setVelocityY(current->getIsOnGround() ? 0.0f : current->getVelocityY());
        next->setIsFacingLeft(current->getIsFacingLeft());
        next->setIsOnGround(current->getIsOnGround());
        next->setIsFlying(current->getIsFlying());
    }

    if (next) {
        next->setIsActivePlayer(true);
        if (next->isMarco()) {
            SoundManager::playSound("sound/marco_select.ogg");
        } else if (next->isEri()) {
            SoundManager::playSound("sound/eri_select.ogg");
        } else if (next->isFio()) {
            SoundManager::playSound("sound/fio_select.ogg");
        }
    }
}

void EntityManager::trySummonFusionCompanion() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
        if (!fusionCompanion) {
            fusionCompanion = new FusionCompanion(players, playerCount, true);
        }
        PlayerSoldier* active = getActivePlayer();
        if (active) {
            fusionCompanion->tryActivate(active->getPosX(), active->getPosY());
        }
    }
}

long long EntityManager::update(Level* level, float dt) {
    PlayerSoldier* active = getActivePlayer();
    trySummonFusionCompanion();

    for (int i = 0; i < playerCount; i++) {
        if (players[i]) {
            Entity* nearest = getNearestEnemy(players[i]->getPosX(), players[i]->getPosY());
            players[i]->setTarget(nearest ? nearest->asTarget() : nullptr);
        }
    }
    for (int i = 0; i < playerCount; i++)
        if (players[i] && players[i] != active)
            players[i]->setCompanionTarget(active);

    // Throttle expensive per-enemy target scan: only update every 6 frames (~10Hz at 60fps)
    static int targetUpdateCounter = 0;
    bool doEnemyTargetUpdate = (++targetUpdateCounter % 6 == 0);

    for (int i = 0; i < count; i++) {
        Entity* e = entities[i];
        if (!e) continue;
        bool isVehicle = (e->getAsVehicle() != nullptr);
        if (!neatAgentsActive && e->isPlayer() && e != active && !isVehicle) continue;
        e->clearPendingShot();
        if (e->isEnemy() && doEnemyTargetUpdate)
            e->setTarget(getNearestPlayer(e->getPosX(), e->getPosY()));
        if (e->getIsAlive())
            e->update(level, dt);
    }

    handleCollisions(dt);

    static const int MAX_PENDING = 128;
    Entity* pendingBuf[MAX_PENDING];
    int pendingCount = 0;

    for (int i = 0; i < count; i++) {
        Entity* e = entities[i];
        if (!e) continue;
        Entity* shot = e->getPendingShot();
        Entity* spawn = e->getPendingSpawnEntity();
        if (shot  && pendingCount < MAX_PENDING) pendingBuf[pendingCount++] = shot;
        if (spawn && pendingCount < MAX_PENDING) pendingBuf[pendingCount++] = spawn;
        if (shot)  e->clearPendingShot();
        if (spawn) e->clearPendingSpawnEntity();
    }

    for (int i = 0; i < pendingCount; i++) addEntity(pendingBuf[i]);

    if (fusionCompanion && fusionCompanion->isActive() && active) {
        fusionCompanion->followPlayer(active->getPosX(), active->getPosY());
        fusionCompanion->scanForTarget(entities, count);
        fusionCompanion->attackNearestEnemy();
        fusionCompanion->update(level, dt);

        Entity* shot = fusionCompanion->getPendingShot();
        if (shot) {
            addEntity(shot);
            fusionCompanion->clearPendingShot();
        }
    }

    return removeDeadEntities(level);
}

void EntityManager::draw(RenderWindow& window) {
    Entity* active = getActivePlayer();
    for (int i = 0; i < count; i++) {
        if (entities[i] && entities[i]->getIsAlive()) {
            // Always draw vehicles; only suppress non-active non-vehicle players
            bool isVehicle = (entities[i]->getAsVehicle() != nullptr);
            if (!neatAgentsActive && entities[i]->isPlayer() && entities[i] != active && !isVehicle)
                continue;
            entities[i]->draw(window);
        }
    }
    if (fusionCompanion) {
        fusionCompanion->draw(window);
    }
}

bool EntityManager::checkAABB(float l1, float r1, float t1, float b1, float l2, float r2, float t2, float b2) {
    return l1 < r2 && r1 > l2 && t1 < b2 && b1 > t2;
}

bool EntityManager::checkAABB(Entity *a, Entity *b) {
    return checkAABB(a->getPosX(), a->getPosX() + a->getWidth(), a->getPosY(),
                     a->getPosY() + a->getHeight(), b->getPosX(),
                     b->getPosX() + b->getWidth(), b->getPosY(),
                     b->getPosY() + b->getHeight());
}

bool EntityManager::shouldCheckCollision(Entity *a, Entity *b) {
    if (a->getIsDying() || b->getIsDying()) return false;
    // If a player soldier is occupied, they are protected inside a vehicle and shouldn't collide with anything
    PlayerSoldier* psA = a->getAsPlayerSoldier();
    if (psA && psA->getIsOccupied()) return false;
    PlayerSoldier* psB = b->getAsPlayerSoldier();
    if (psB && psB->getIsOccupied()) return false;

    int fa = a->getFaction();
    int fb = b->getFaction();
    
    if (fa == fb) {
        bool aIsVehicle = (a->getAsVehicle() != nullptr);
        bool bIsVehicle = (b->getAsVehicle() != nullptr);
        bool aIsPlayerSoldier = (a->getAsPlayerSoldier() != nullptr);
        bool bIsPlayerSoldier = (b->getAsPlayerSoldier() != nullptr);
        if ((aIsVehicle && bIsPlayerSoldier) || (bIsVehicle && aIsPlayerSoldier)) {
            return true;
        }
        return false;
    }
    
    if (fa == FACTION_ENEMY && fb == FACTION_NEUTRAL) return false;
    if (fa == FACTION_NEUTRAL && fb == FACTION_ENEMY) return false;
    return true;
}

void EntityManager::handleCollisions(float dt) {
    for (int i = 0; i < count; i++) {
        Entity *e1 = entities[i];
        if (!e1 || !e1->getIsAlive()) continue;
        for (int j = i + 1; j < count; j++) {
            Entity *e2 = entities[j];
            if (!e2 || !e2->getIsAlive()) continue;
            if (!shouldCheckCollision(e1, e2)) continue;
            if (checkAABB(e1, e2)) {
                e1->onCollideWith(e2, dt);
                if (e1->getIsAlive()) {
                    e2->onCollideWith(e1, dt);
                }
            }
        }
    }
}

void EntityManager::processSpawns(Level* level) {
    if (!level) return;
    if (allSpawnsProcessed && level->getLevelId() != 99) return;   // O(1) early exit once done

    SpawnPoint* table = level->getSpawnTable();
    int spawnCount = level->getSpawnCount();
    int bSz = level->getBlockSize();

    bool anyUnspawned = false;
    for (int i = 0; i < spawnCount; i++) {
        if (table[i].isSpawned()) continue;
        anyUnspawned = true;

        const char* type = table[i].getEntityType();
        double spawnX = (table[i].getCol() * bSz);
        Camera* cam = level->getCamera();
        if (cam) {
            double camX  = cam->getCamX();
            double screenW = cam->getScreenW();
            if (spawnX < camX - 1280 || spawnX > camX + screenW + 1280) continue;
        }
        double spawnY = (table[i].getRow() * bSz);
        Entity* e = nullptr;
        for (int s = 0; s < spawnerCount; s++) {
            if (areEqual(spawnerRegistry[s]->tag(), type)) {
                e = spawnerRegistry[s]->make(spawnX, spawnY);
                break;
            }
        }
        if (e) {
            if (!areEqual(type, "paratrooper") && !areEqual(type, "alien") &&
                !areEqual(type, "flying_tara") && !areEqual(type, "enemy_sub") &&
                !areEqual(type, "hairbuster") && !areEqual(type, "sea_satan")) {
                int groundRow = level->getFirstSolidRow(table[i].getCol());
                double groundTopY = ((groundRow + 1) * bSz);
                e->setPosY(groundTopY - e->getHeight());
            }
            addEntity(e);
            table[i].setSpawned(true);
        }
    }

    // If nothing is left to spawn, lock the gate (unless in infinite level)
    if (!anyUnspawned && level->getLevelId() != 99)
        allSpawnsProcessed = true;
}



const float AlienEnemy::BEAM_CHARGE_TIME = 2;
const float AlienEnemy::HOVER_HEIGHT = 500;
Texture HairbusterRiberts::texture;
bool HairbusterRiberts::textureLoaded = false;
Texture IronNokana::texture;
bool IronNokana::textureLoaded = false;
Texture SeaSatan::texture;
bool SeaSatan::textureLoaded = false;
Texture MummyWarrior::mummyTexture;
bool MummyWarrior::mummyTextureLoaded = false;
const float BazookaSoldier::FIRE_COOLDOWN = 3;
const float GrenadeSoldier::FIRE_COOLDOWN = 2;
const float SlugMariner::SHOOT_ANIM_DURATION = 5 * 0.08f;
const float M15ABradley::SHOOT_ANIM_DURATION = 7 * 0.08f;
