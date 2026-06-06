#pragma once
#include "../Level and Environment System/perlin.h"
#include "../headers/headers.h"
#include "../headers/string.h"
#include "GameState.h"

// Loading screen state
class LoadingState : public GameState {
private:
  float loadingTimer;
  int loadingFrame;
  float loadingTimeTotal;
  Texture texLoading;
  Sprite loadingSprite;
  Font font;
  Text loadingLabel;
  IntRect frames[10];
  float sw, sh;
  char mode[32];
  int level;
  bool done;

public:
  LoadingState(float sw, float sh, const char *m, int lv)
      : sw(sw), sh(sh), level(lv), loadingTimer(0), loadingFrame(0),
        loadingTimeTotal(0), done(false) {
    copyStringN(mode, m, 32);

    texLoading.loadFromFile("main menu/loading ufxc.png");
    loadingSprite.setTexture(texLoading);
    loadingSprite.setScale(0.3, 0.3);

    font.loadFromFile("Founts/Metal-Slug-Latino-Regular.ttf");
    loadingLabel.setFont(font);
    loadingLabel.setString("LOADING");
    loadingLabel.setCharacterSize(40);
    loadingLabel.setFillColor(Color::White);

    // Center the origin of the text
    FloatRect textRect = loadingLabel.getLocalBounds();
    loadingLabel.setOrigin(textRect.left + textRect.width / 2.0f,
                           textRect.top  + textRect.height / 2.0f);

    IntRect f[10] = {{284, 208, 3026, 328},   {284, 669, 3027, 340},
                     {287, 1145, 3023, 338},  {284, 1618, 3026, 338},
                     {284, 2088, 3025, 340},  {3602, 208, 3004, 329},
                     {3602, 670, 3004, 339},  {3602, 1145, 3004, 337},
                     {3602, 1618, 3004, 338}, {3602, 2089, 3004, 339}};
    for (int i = 0; i < 10; i++)
      frames[i] = f[i];
  }

  void handleInput() override {}
  void update(float dt) override {
    loadingTimer += dt;
    loadingTimeTotal += dt;
    if (loadingTimer > 0.1) {
      loadingFrame = (loadingFrame + 1) % 10;
      loadingTimer = 0;
    }
    if (loadingTimeTotal > 3) {
      done = true;
    }
  }
  void draw(RenderWindow &window) override {
    window.clear(Color::Black);
    loadingSprite.setTextureRect(frames[loadingFrame]);
    loadingSprite.setPosition(sw / 2 - (frames[loadingFrame].width * 0.3) / 2,
                              400);
    window.draw(loadingSprite);

    loadingLabel.setPosition(sw / 2.0f, 540.0f);
    window.draw(loadingLabel);
  }

  bool isLoadingDone() const override { return done; }
  const char *getSelectedMode() const override { return mode; }
  int getSavedLevel() const override { return level; }
};
