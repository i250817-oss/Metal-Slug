#pragma once
#include "../Character System/assignement q4.h"
#include "../Collectible and Interactable System/POWPrisoner.h"
#include "../Collectible and Interactable System/SupplyBox.h"
#include "../Level and Environment System/camera.h"
#include "../Level and Environment System/perlin.h"
#include "../Manager/EntityManager.h"
#include "../Manager/LevelManager.h"
#include "../ScoreCard/PlayerScoreCard.h"
#include "../headers/string.h"
#include "DamageOverlay.h"
#include "GameState.h"
#include "LevelFactory.h"
#include "PlayerFactory.h"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

// Main gameplay state
class PlayState : public GameState {
private:
  EntityManager entityManager;
  LevelManager levelManager;
  Camera camera;
  bool fusionFontLoaded;
  PlayerScoreCard *activeScoreCard;
  Text fusionHUDText;
  Text hudScoreText;
  Text hudPlayerText;
  char playerName[64];

  bool zWasPressed;
  bool nWasPressed;
  const float screenWidth;
  const float screenHeight;

  bool cheatKeyWasPressed[26];
  int pledgeProgress;
  int meowProgress;

  Text cheatNotifyText;
  float cheatNotifyTimer;
  float cheatNotifyX;

  Text immortalStatusText;
  float immortalStatusTimer;
  float immortalStatusX;

  Music levelMusic;
  bool isPlayerUnderwater;
  bool musicPlaying;
  bool muffledGenerated;
  bool muffledBufferLoaded;
  bool playerWasDead;
  bool levelCompleteFlag;
  bool gameOverFlag;
  char currentMode[32];
  int currentLevel;
  Font font;
  int lastEnemyCount;

public:
  PlayState(float sw, float sh, const char *mode = "Campaign",
            int startLevel = 1)
      : camera(sw, sh), zWasPressed(false), nWasPressed(false), screenWidth(sw), screenHeight(sh),
        musicPlaying(false), levelCompleteFlag(false), gameOverFlag(false), currentLevel(startLevel),
        pledgeProgress(0),
        meowProgress(0), cheatNotifyTimer(0), cheatNotifyX(0),
        immortalStatusTimer(0), immortalStatusX(0), isPlayerUnderwater(false),
        muffledGenerated(false), muffledBufferLoaded(false),
        playerWasDead(false), lastEnemyCount(0), activeScoreCard(nullptr) {
    copyStringN(currentMode, mode, 32);

    copyStringN(playerName, "PLAYER", 64);
    ifstream nameFile("player_save.txt");
    if (nameFile.is_open()) {
      nameFile >> playerName;
      nameFile.close();
    }
    activeScoreCard = new PlayerScoreCard(playerName);
    ScoreCard::activeInstance = activeScoreCard;

    for (int i = 0; i < 26; i++)
      cheatKeyWasPressed[i] = false;

    fusionFontLoaded =
        font.loadFromFile("Founts/Metal-Slug-Latino-Regular.ttf");
    if (fusionFontLoaded) {
      fusionHUDText.setFont(font);
      fusionHUDText.setCharacterSize(18);
      fusionHUDText.setFillColor(Color::Cyan);
      fusionHUDText.setPosition(10, 120);

      hudPlayerText.setFont(font);
      hudPlayerText.setCharacterSize(24);
      hudPlayerText.setFillColor(Color(255, 215, 0));
      hudPlayerText.setPosition(10, 50);

      hudScoreText.setFont(font);
      hudScoreText.setCharacterSize(24);
      hudScoreText.setFillColor(Color(255, 215, 0));
      hudScoreText.setPosition(10, 80);

      cheatNotifyText.setFont(font);
      cheatNotifyText.setCharacterSize(40);
      cheatNotifyText.setFillColor(Color::Yellow);
      cheatNotifyText.setOutlineColor(Color::Black);
      cheatNotifyText.setOutlineThickness(2);

      immortalStatusText.setFont(font);
      immortalStatusText.setCharacterSize(20);
      immortalStatusText.setFillColor(Color::Red);
      immortalStatusText.setPosition(10, 150);
    }
    const float spawnX = 400;
    const float spawnY = 500;

    PlayerFactory::createAll(entityManager, spawnX, spawnY);

    if (areEqual(mode, "Campaign"))
      LevelFactory::createCampaign(levelManager);
    else if (areEqual(mode, "Survival"))
      LevelFactory::createSurvival(levelManager);
    else if (areEqual(mode, "Infinity"))
      LevelFactory::createInfinity(levelManager);
    else
      LevelFactory::createCampaign(levelManager);

    // Clamp startLevel so we never advance past the last available level
    int totalLevels = levelManager.getLevelCount();
    int targetIndex = (startLevel - 1 < totalLevels) ? (startLevel - 1) : (totalLevels - 1);
    for (int i = 0; i < targetIndex; i++) {
      levelManager.nextLevel();
    }

    Level *current = levelManager.getCurrentLevel();
    if (current) {
      current->setCamera(&camera);
      current->load();
      entityManager.processSpawns(current);
      lastEnemyCount = entityManager.getEnemyCount();
    }

    // Choose the music track based on mode and level index
    const char *musicTrack = "sound/1-07. Assault Theme (Stage 3).mp3"; // default for level 1
    if (areEqual(mode, "Campaign") || areEqual(mode, "Survival")) {
      if (startLevel == 2) {
        musicTrack = "sound/1-08. Ridge 256 (Stage 4).mp3";
      } else if (startLevel == 3) {
        musicTrack = "sound/1-09. Gerhardt City (Stage 5).mp3";
      } else if (startLevel == 4) {
        musicTrack = "sound/1-10. Final Attack (Stage 6).mp3";
      }
    } else if (areEqual(mode, "Infinity")) {
      musicTrack = "sound/1-08. Ridge 256 (Stage 4).mp3";
    }

    if (levelMusic.openFromFile(musicTrack)) {
      levelMusic.setVolume(100);
      levelMusic.setLoop(true);
      levelMusic.play();
      musicPlaying = true;
    }
  }

  ~PlayState() {
    levelMusic.stop();
    delete activeScoreCard;
  }

  void handleInput() override {}

  void update(float dt) override {
    Level *current = levelManager.getCurrentLevel();
    if (!current)
      return;
    current->update(dt);

    bool zNow = Keyboard::isKeyPressed(Keyboard::Z);
    if (zNow && !zWasPressed)
      entityManager.cycleActivePlayer();
    zWasPressed = zNow;

    bool nNow = Keyboard::isKeyPressed(Keyboard::N);
    if (nNow && !nWasPressed)
      entityManager.toggleNeatAgents();
    nWasPressed = nNow;

    entityManager.update(current, dt);
    entityManager.processSpawns(current);

    PlayerSoldier *p = entityManager.getActivePlayer();
    if (p) {
      float centreX = p->getPosX() + p->getWidth() / 2;
      float centreY = p->getPosY() + p->getHeight() / 2;
      camera.followCamera(centreX, centreY, current->getWorldPixelW(),
                          current->getWorldPixelH());

      int playerCol = (centreX / current->getBlockSize());
      if (playerCol >= current->getWidth() - 5 &&
          entityManager.getEnemyCount() == 0 && current->canLevelEnd()) {
        current->setIsComplete(true);
      }
      isPlayerUnderwater = (current->getBiomeAt(centreX) == 1);
    }

    if (current->getIsComplete() && !levelCompleteFlag) {
      levelCompleteFlag = true;
      levelMusic.stop();
      musicPlaying = false;
    }

    for (int i = 0; i < 26; i++) {
      Keyboard::Key k = (Keyboard::Key)(Keyboard::A + i);
      bool down = Keyboard::isKeyPressed(k);
      if (down && !cheatKeyWasPressed[i]) {
        char c = 'A' + i;
        const char *mCode = "MEOW";
        if (c == mCode[meowProgress]) {
          meowProgress++;
          if (mCode[meowProgress] == '\0') {
            activateMeow();
            meowProgress = 0;
          }
        } else {
          meowProgress = 0;
        }
        const char *pCode = "PLEDGE";
        if (c == pCode[pledgeProgress]) {
          pledgeProgress++;
          if (pCode[pledgeProgress] == '\0') {
            activatePledge();
            pledgeProgress = 0;
          }
        } else {
          pledgeProgress = 0;
        }
      }
      cheatKeyWasPressed[i] = down;
    }

    if (cheatNotifyTimer > 0)
      cheatNotifyTimer -= dt;
    if (immortalStatusTimer > 0)
      immortalStatusTimer -= dt;

    bool allPlayersDead = (entityManager.getPlayerCount() > 0);
    for (int i = 0; i < entityManager.getPlayerCount(); i++) {
      PlayerSoldier *ps = entityManager.getPlayerAt(i);
      if (ps && ps->getIsAlive()) {
        allPlayersDead = false;
        break;
      }
    }
    if (allPlayersDead) {
      gameOverFlag = true;
      levelMusic.stop();
      musicPlaying = false;
    }
  }

  void activateMeow() {
    for (int i = 0; i < entityManager.getPlayerCount(); i++) {
      PlayerSoldier *p = entityManager.getPlayerAt(i);
      p->unlockAllWeapons();
      p->activateSpecial();
    }
    cheatNotifyText.setString("MEOW CHEAT ACTIVATED: FULL ARSENAL");
    cheatNotifyTimer = 3;
    cheatNotifyX = (screenWidth - 600) / 2;
  }

  void activatePledge() {
    PlayerSoldier *p = entityManager.getActivePlayer();
    if (p)
      p->setImmortal(true);
    cheatNotifyText.setString("PLEDGE CHEAT ACTIVATED: GOD MODE");
    cheatNotifyTimer = 3;
    cheatNotifyX = (screenWidth - 600) / 2;
  }

  void draw(RenderWindow &window) override {
    View view(FloatRect(camera.getCamX(), camera.getCamY(), camera.getScreenW(),
                        camera.getScreenH()));
    View oldView = window.getView();
    window.setView(view);
    Level *current = levelManager.getCurrentLevel();
    if (current)
      current->draw(window);
    entityManager.draw(window);
    window.setView(oldView);

    PlayerSoldier *pActive = entityManager.getActivePlayer();
    if (pActive) {
      pActive->getDamageOverlay().draw(window, screenWidth, screenHeight);
    }

    if (activeScoreCard && fusionFontLoaded) {
      char sBuf[32];
      intToString(activeScoreCard->getScore(), sBuf);
      // Build HUD strings on the stack — no heap allocation every frame
      char pStr[96] = "Player: ";
      char sStr[96] = "Score:  ";
      int pLen = 8; // len of "Player: "
      for (int k = 0; playerName[k] && pLen < 95; k++) pStr[pLen++] = playerName[k];
      pStr[pLen] = '\0';
      int sLen = 8; // len of "Score:  "
      for (int k = 0; sBuf[k] && sLen < 95; k++) sStr[sLen++] = sBuf[k];
      sStr[sLen] = '\0';
      hudPlayerText.setString(pStr);
      hudScoreText.setString(sStr);
      window.draw(hudPlayerText);
      window.draw(hudScoreText);

      //Temporary: Show current block coordinates
      if (pActive && current) {
          int col = pActive->getPosX() / current->getBlockSize();
          int row = pActive->getPosY() / current->getBlockSize();
          char colB[16], rowB[16];
          intToString(col, colB);
          intToString(row, rowB);
          char coordBuf[64] = "COL: ";
          int ci = 5;
          for (int k = 0; colB[k] && ci < 60; k++) coordBuf[ci++] = colB[k];
          coordBuf[ci++] = ' '; coordBuf[ci++] = 'R'; coordBuf[ci++] = 'O';
          coordBuf[ci++] = 'W'; coordBuf[ci++] = ':'; coordBuf[ci++] = ' ';
          for (int k = 0; rowB[k] && ci < 63; k++) coordBuf[ci++] = rowB[k];
          coordBuf[ci] = '\0';
          fusionHUDText.setString(coordBuf);
          fusionHUDText.setPosition(10, 120);
          window.draw(fusionHUDText);
      }
    }

    if (cheatNotifyTimer > 0) {
      cheatNotifyText.setPosition(cheatNotifyX, 100);
      window.draw(cheatNotifyText);
    }

    PlayerSoldier *p = entityManager.getActivePlayer();
    if (p && p->getIsInvincible()) {
      immortalStatusText.setString("IMMORTAL MODE ACTIVE");
      window.draw(immortalStatusText);
    }
  }

  const char *getStateName() const { return "PlayState"; }

  bool isLevelComplete() const override { return levelCompleteFlag; }
  bool isGameOver() const override { return gameOverFlag; }
  const char* getSelectedMode() const override { return currentMode; }
  int getSavedLevel() const override { return currentLevel; }
  PlayerScoreCard* getScoreCard() const override { return activeScoreCard; }

  // Dev terminal injection: expose active player
  PlayerSoldier* getActivePlayer() const override {
    return entityManager.getActivePlayer();
  }
};
