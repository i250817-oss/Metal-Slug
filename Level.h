#pragma once
#include "../headers/headers.h"
#include "camera.h"

using namespace sf;



class Level {
public:
  virtual ~Level() {}

  //attach camera
  virtual void setCamera(Camera *cam) = 0;
  virtual Camera *getCamera() const = 0;

  //level setup
  virtual void load() = 0;

  //per-frame logic
  virtual void update(float dt) = 0;

  //rendering
  virtual void draw(RenderWindow& window) = 0;

  //dimensions
  virtual int getLevelId() const = 0;
  virtual int getWidth() const = 0;       //in blocks
  virtual int getHeight() const = 0;      //in blocks
  virtual int getWorldPixelW() const = 0; //width  * blockSize
  virtual int getWorldPixelH() const = 0; //height * blockSize

  //completion
  virtual bool getIsComplete() const = 0;
  virtual void setIsComplete(bool value) = 0;

  //collision query
  //returns true when the block at (col, row) is solid (player cannot pass).
  virtual bool isSolidAt(int col, int row) const = 0;
  virtual bool isPlatformAt(int col, int row) const = 0;

  //returns the pixel size of one block (32 in LevelBase)
  virtual int getBlockSize() const = 0;

  //spawns
  virtual class SpawnPoint *getSpawnTable() = 0;
  virtual int getSpawnCount() const = 0;

  //biome query
  //returns 0=Plains, 1=Aquatic, 2=Aerial
  virtual int getBiomeAt(double worldX) const = 0;
  virtual int getGroundRow(int col) = 0;
  virtual int getFirstSolidRow(int col) const = 0;

  //animations
  virtual void startStairAnimation() = 0;

  //phase check
  virtual bool canLevelEnd() const { return true; }

  //kill report
  virtual void reportKill(const char * type) {}
};
;