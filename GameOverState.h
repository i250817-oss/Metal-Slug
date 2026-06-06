#pragma once
#include "../ScoreCard/PlayerScoreCard.h"
#include "../headers/headers.h"
#include "../headers/string.h"
#include "GameState.h"
#include <cmath>

class GameOverState : public GameState {
private:
  float sw, sh;
  Font font;
  Text gameOverTitle;
  Text scoreText;
  Text optionRetry;
  Text optionMenu;
  
  int selectedIndex;
  float inputCooldown;
  float animationTimer;
  
  bool retryChosen;
  bool menuChosen;
  char mode[32];
  int level;
  int finalScore;

public:
  GameOverState(float sw, float sh, PlayerScoreCard *sc, const char* m, int lv)
      : sw(sw), sh(sh), level(lv), selectedIndex(0), inputCooldown(0.0f),
        animationTimer(0.0f), retryChosen(false), menuChosen(false) {
    copyStringN(mode, m, 32);
    finalScore = sc ? sc->getScore() : 0;

    font.loadFromFile("Founts/Metal-Slug-Latino-Regular.ttf");

    // pulsating GAME OVER title
    gameOverTitle.setFont(font);
    gameOverTitle.setString("GAME OVER");
    gameOverTitle.setCharacterSize(80);
    gameOverTitle.setFillColor(Color(220, 20, 60)); // Crimson red
    gameOverTitle.setOutlineColor(Color::Black);
    gameOverTitle.setOutlineThickness(4);
    
    // Score text
    char sBuf[32];
    intToString(finalScore, sBuf);
    char* fullScoreStr = concatenate("FINAL SCORE: ", sBuf);
    scoreText.setFont(font);
    scoreText.setString(fullScoreStr);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color(255, 215, 0)); // Gold
    scoreText.setOutlineColor(Color::Black);
    scoreText.setOutlineThickness(2);
    delete[] fullScoreStr;

    // Retry option
    optionRetry.setFont(font);
    optionRetry.setString("RETRY LEVEL");
    optionRetry.setCharacterSize(26);
    optionRetry.setOutlineColor(Color::Black);
    optionRetry.setOutlineThickness(2);

    // Menu option
    optionMenu.setFont(font);
    optionMenu.setString("MAIN MENU");
    optionMenu.setCharacterSize(26);
    optionMenu.setOutlineColor(Color::Black);
    optionMenu.setOutlineThickness(2);
  }

  void handleInput() override {}

  void update(float dt) override {
    inputCooldown += dt;
    animationTimer += dt;

    if (inputCooldown >= 0.18f) {
      if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
        selectedIndex = (selectedIndex - 1 + 2) % 2;
        inputCooldown = 0.0f;
      } else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
        selectedIndex = (selectedIndex + 1) % 2;
        inputCooldown = 0.0f;
      } else if (Keyboard::isKeyPressed(Keyboard::Enter) || Keyboard::isKeyPressed(Keyboard::Space)) {
        if (selectedIndex == 0) {
          retryChosen = true;
        } else {
          menuChosen = true;
        }
        inputCooldown = 0.0f;
      }
    }
  }

  void draw(RenderWindow &window) override {
    // Elegant dark red gradient overlay background
    window.clear(Color(20, 5, 5));

    // Dynamic title pulsing scaling effect using sine wave
    float scale = 1.0f + 0.08f * std::sin(animationTimer * 3.5f);
    gameOverTitle.setScale(scale, scale);
    
    // Center alignment helper
    auto centerText = [&](Text &t, float y) {
      FloatRect rect = t.getLocalBounds();
      t.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
      t.setPosition(sw / 2.0f, y);
    };

    centerText(gameOverTitle, sh * 0.3f);
    centerText(scoreText, sh * 0.45f);

    // Option indicators and colors based on selection
    if (selectedIndex == 0) {
      optionRetry.setFillColor(Color::Yellow);
      optionRetry.setString("> RETRY LEVEL <");
      optionMenu.setFillColor(Color(160, 160, 160));
      optionMenu.setString("MAIN MENU");
    } else {
      optionRetry.setFillColor(Color(160, 160, 160));
      optionRetry.setString("RETRY LEVEL");
      optionMenu.setFillColor(Color::Yellow);
      optionMenu.setString("> MAIN MENU <");
    }

    centerText(optionRetry, sh * 0.62f);
    centerText(optionMenu, sh * 0.72f);

    window.draw(gameOverTitle);
    window.draw(scoreText);
    window.draw(optionRetry);
    window.draw(optionMenu);
  }

  bool isGameOverRetry() const override { return retryChosen; }
  bool isGameOverMenu() const override { return menuChosen; }

  const char *getSelectedMode() const override { return mode; }
  int getSavedLevel() const override { return level; }
};
