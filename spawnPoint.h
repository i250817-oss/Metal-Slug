#pragma once

using namespace std;

//spawnPoint
//one entry in a level's baked spawn table.
//entityType  string tag matched by EntityManager ("infantry", "zombie", etc.)
//col / row    grid coordinates where the entity will be placed
//biomeZone    0=aerial  1=plains  2=aquatic  (mirrors Block biome constants)
//spawned     flipped to true after EntityManager has created the entity
//

class SpawnPoint {
private:
  char entityType[32];
  int col;
  int row;
  int biomeZone;
  bool spawned;

public:
  SpawnPoint() : col(0), row(0), biomeZone(0), spawned(false) {
    entityType[0] = '\0';
  }

  //Setters
  void setEntityType(const char *type) {
    int i = 0;
    while (type[i] != '\0' && i < 31) {
      entityType[i] = type[i];
      i++;
    }
    entityType[i] = '\0';
  }
  void setCol(int c) { col = c; }
  void setRow(int r) { row = r; }
  void setBiomeZone(int z) { biomeZone = z; }
  void setSpawned(bool s) { spawned = s; }

  //Getters
  const char *getEntityType() const { return entityType; }
  int getCol() const { return col; }
  int getRow() const { return row; }
  int getBiomeZone() const { return biomeZone; }
  bool isSpawned() const { return spawned; }
};
