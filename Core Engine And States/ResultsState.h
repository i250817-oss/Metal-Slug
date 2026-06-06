#pragma once
#include "../Level and Environment System/perlin.h"
#include "../ScoreCard/PlayerScoreCard.h"
#include "../headers/headers.h"
#include "../headers/string.h"
#include "GameState.h"

// Level results state
class ResultsState : public GameState {
private:
  float resultsTimer;
  Texture texImage6;
  Sprite spriteImage6;
  Font font;
  Text scoreStatsText;
  float sw, sh;
  char mode[32];
  int nextLevel;
  bool done;

  int rebelKills;
  int zombieKills;
  int vehicleKills;
  int otherKills;
  int finalScore;

public:
  ResultsState(float sw, float sh, PlayerScoreCard *sc, const char* m, int nextLv)
      : sw(sw), sh(sh), nextLevel(nextLv), resultsTimer(0), done(false) {
    copyStringN(mode, m, 32);

    if (sc) {
      rebelKills = sc->getRebelKills();
      zombieKills = sc->getZombieKills();
      vehicleKills = sc->getVehicleKills();
      otherKills = sc->getOtherKills();
      finalScore = sc->getScore();
    } else {
      rebelKills = zombieKills = vehicleKills = otherKills = finalScore = 0;
    }

    if (texImage6.loadFromFile("main menu/image6.png")) {
      spriteImage6.setTexture(texImage6);
      spriteImage6.setTextureRect(IntRect(16, 13, 274, 206));
      spriteImage6.setScale(2, 2);
    }

    font.loadFromFile("Founts/Metal-Slug-Latino-Regular.ttf");
    scoreStatsText.setFont(font);
    scoreStatsText.setCharacterSize(24);
    scoreStatsText.setFillColor(Color::White);

    float px = (perlin(rand() % 100, rand() % 100) + 1) * 0.5;
    float py = (perlin(rand() % 100, rand() % 100) + 1) * 0.5;
    spriteImage6.setPosition(px * (sw - 274 * 2), py * (sh - 206 * 2) - 100);
  }

  void handleInput() override {
    if (resultsTimer > 2 && Keyboard::isKeyPressed(Keyboard::Space)) {
      done = true;
    }
  }
  void update(float dt) override {
    resultsTimer += dt;
    if (resultsTimer > 8) {
      done = true;
    }
  }
  void draw(RenderWindow &window) override {
    window.clear(Color(0, 0, 139));
    int colorPhase = (int)(resultsTimer * 4) % 3;
    if (colorPhase == 0)
      spriteImage6.setColor(Color::Yellow);
    else if (colorPhase == 1)
      spriteImage6.setColor(Color(255, 105, 180));
    else
      spriteImage6.setColor(Color::Green);
    window.draw(spriteImage6);

    float startX = spriteImage6.getPosition().x + 20;
    float startY = spriteImage6.getPosition().y + 15;
    float lineH  = 32;

    // Helper lambda: draw one stat line at a given Y
    auto drawLine = [&](const char* label, int value, float y, Color col) {
      char buf[32];
      intToString(value, buf);
      char* str = concatenate(label, buf);
      scoreStatsText.setString(str);
      scoreStatsText.setFillColor(col);
      scoreStatsText.setPosition(startX, y);
      window.draw(scoreStatsText);
      delete[] str;
    };

    if (resultsTimer > 0.4f) {
      scoreStatsText.setCharacterSize(26);
      scoreStatsText.setString("LEVEL CLEARED!");
      scoreStatsText.setFillColor(Color::Yellow);
      scoreStatsText.setPosition(startX, startY);
      window.draw(scoreStatsText);
    }
    if (resultsTimer > 1.2f) {
      scoreStatsText.setCharacterSize(22);
      drawLine("Infantry Kills:  ", rebelKills,  startY + lineH * 1.5f, Color::White);
    }
    if (resultsTimer > 1.8f) {
      scoreStatsText.setCharacterSize(22);
      drawLine("Undead Kills:    ", zombieKills, startY + lineH * 2.5f, Color(180, 230, 180));
    }
    if (resultsTimer > 2.4f) {
      scoreStatsText.setCharacterSize(22);
      drawLine("Vehicle Kills:   ", vehicleKills, startY + lineH * 3.5f, Color(180, 220, 255));
    }
    if (resultsTimer > 3.0f) {
      scoreStatsText.setCharacterSize(22);
      drawLine("Other Kills:     ", otherKills,  startY + lineH * 4.5f, Color(255, 200, 120));
    }
    if (resultsTimer > 3.8f) {
      scoreStatsText.setCharacterSize(26);
      drawLine("TOTAL SCORE:  ", finalScore, startY + lineH * 6.0f, Color::Yellow);
    }
    if (resultsTimer > 4.5f) {
      scoreStatsText.setCharacterSize(18);
      scoreStatsText.setString("SPACE to continue...");
      scoreStatsText.setFillColor(Color(200, 200, 200));
      scoreStatsText.setPosition(startX, startY + lineH * 7.2f);
      window.draw(scoreStatsText);
    }
  }

  bool isResultsDone() const override { return done; }
  const char *getSelectedMode() const override { return mode; }
  int getSavedLevel() const override { return nextLevel; }
};

