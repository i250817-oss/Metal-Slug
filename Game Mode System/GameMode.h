#pragma once
#include "../Character System/PlayerSoldier.h"

// abstract base for both game modes (Survival / Campaign).
class GameMode {
private:
  PlayerSoldier *characters[4];
  int activeCharIndex;
  int totalScore;
  bool isRunning;
  bool devModeActive;

public:
  GameMode(PlayerSoldier *chars[], int count);
  virtual ~GameMode();

  PlayerSoldier *getActiveCharacter() const;
  int getTotalScore() const;
  void addScore(int val);
  bool getIsRunning() const;
  bool getDevModeActive() const;

  void switchCharacter();
  void activateDevMode();

  virtual void start() = 0;
  virtual void update(float dt) = 0;
  virtual void end() = 0;
};