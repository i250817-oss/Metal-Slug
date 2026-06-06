#pragma once
#include "../Entity Root/Entity.h"
#include <cmath>
#include <cstdlib>

using namespace sf;
using namespace std;

class PlayerSoldier;
class FusionCompanion;
class Level;

// enemy spawner base
class EnemySpawner {
public:
  virtual const char *tag() const = 0;
  virtual Entity *make(double x, double y) const = 0;
  virtual ~EnemySpawner() {}
};

// entityManager
class EntityManager {
private:
  Entity **entities;
  int count;
  int capacity;

  PlayerSoldier *players[4];
  int playerCount;
  int activePlayerIndex;
  bool neatAgentsActive;

  FusionCompanion *fusionCompanion;

  static const int MAX_SPAWNERS = 32;
  EnemySpawner *spawnerRegistry[MAX_SPAWNERS];
  int spawnerCount;

  // Performance cache: avoid O(n) enemy count scan every frame
  int  cachedEnemyCount;   // kept accurate via inc/dec
  bool enemyCountDirty;    // set true whenever we must recount

  // Short-circuit processSpawns once every entry is spawned
  bool allSpawnsProcessed;

  EntityManager(const EntityManager &e);
  EntityManager &operator=(const EntityManager &e);

  void resize();

  static EntityManager *instance;

public:
  static EntityManager *getInstance() { return instance; }

  PlayerSoldier *getNearestPlayer(double x, double y);

  Entity *getNearestEnemy(double x, double y);

  EntityManager(int initialCapacity = 256);

  ~EntityManager();

  void addEntity(Entity *e);

  void removeEntityAt(int index);

  long long removeDeadEntities(Level *level);

  void clearEnemies();

  Entity **getEntities() const { return entities; }
  int getCount() const { return count; }
  int getPlayerCount() const { return playerCount; }
  PlayerSoldier *getPlayerAt(int i) const;

  int getEnemyCount();     // cached — O(1) most frames
  void invalidateEnemyCount() { enemyCountDirty = true; }
  void resetAllSpawns()    { allSpawnsProcessed = false; }

  PlayerSoldier *getActivePlayer() const;

  void cycleActivePlayer();

  void toggleNeatAgents() { neatAgentsActive = !neatAgentsActive; }

  void trySummonFusionCompanion();

  void processSpawns(Level *level);

  bool checkAABB(float l1, float r1, float t1, float b1, float l2, float r2,
                 float t2, float b2);

  bool checkAABB(Entity *a, Entity *b);

  bool shouldCheckCollision(Entity *a, Entity *b);

  void handleCollisions(float dt);

  long long update(Level *level, float dt);

  void draw(RenderWindow &window);

  Entity **getEntityArray() { return entities; }
  int getEntityCount() const { return count; }
};
