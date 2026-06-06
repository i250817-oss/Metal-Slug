#pragma once
#include "../Character System/PlayerSoldier.h"
#include "../headers/headers.h"
#include "GameOverState.h"
#include "GameStateManager.h"
#include "LoadingState.h"
#include "PlayState.h"
#include "ResultsState.h"
#include "UrduDeveloperTerminal.h"
#include "VoiceRecognition.h"
#include "mainmenu.h"
using namespace sf;

// Main game controller
class Game {
private:
  RenderWindow window;
  GameStateManager stateManager;
  Clock clock;
  float accumulator;
  UrduDeveloperTerminal devTerminal;
  VoiceRecognitionEngine voiceEngine;

  const float FIXED_DT = 1.0f / 60.0f;
  const float MAX_FRAME_DT = 0.1f;

public:
  Game()
      : window(VideoMode(1600, 900), "Metal Slug - Ahmad And Osaid"),
        accumulator(0.0f) {
    window.setFramerateLimit(60);
  }

  ~Game() {}

  void run() {
    // ── Voice startup gate ──────────────────────────────────────────────────
    runVoiceStartup();

    stateManager.changeState(new MenuState());
    // stateManager.changeState(new PlayState(1600, 900, "Campaign", 1));
    // stateManager.changeState(new PlayState(1600, 900, "Campaign", 4));
    clock.restart();

    while (window.isOpen()) {
      float dt = clock.restart().asSeconds();

      if (dt > MAX_FRAME_DT)
        dt = MAX_FRAME_DT;

      accumulator += dt;

      handleEvents();

      while (accumulator >= FIXED_DT) {
        stateManager.update(FIXED_DT);
        accumulator -= FIXED_DT;
      }

      checkStateTransitions();
      render();
    }
  }

private:
  // ── Voice startup: blocks until phrase detected (skipped if no WAV/mic) ───
  void runVoiceStartup() {
    const char *REF_WAV = "sound/valar_morghulis.wav";

    if (!voiceEngine.loadReference(REF_WAV)) {
      cout << "[VRE] WAV not found at '" << REF_WAV
           << "' — voice gate skipped.\n";
      return;
    }

    bool detected = false;
    while (!detected && window.isOpen()) {
      detected = voiceEngine.listenAndMatch(window);

      if (!detected) {
        // 1.5 s pause then retry
        Clock retryClock;
        while (retryClock.getElapsedTime().asSeconds() < 1.5f &&
               window.isOpen()) {
          Event ev;
          while (window.pollEvent(ev))
            if (ev.type == Event::Closed)
              window.close();
          window.clear(Color::Black);
          window.display();
        }
      }
    }
  }

  void checkStateTransitions() {
    GameState *current = stateManager.getCurrentState();
    if (!current)
      return;

    if (current->isMenu() && current->shouldStartGame()) {
      stateManager.changeState(new LoadingState(
          1600, 900, current->getSelectedMode(), current->getSavedLevel()));

    } else if (!current->isMenu() && current->isLoadingDone()) {
      stateManager.changeState(new PlayState(
          1600, 900, current->getSelectedMode(), current->getSavedLevel()));

    } else if (current->isLevelComplete()) {
      PlayerScoreCard *sc = current->getScoreCard();
      int nextLv = current->getSavedLevel() + 1;
      stateManager.changeState(
          new ResultsState(1600, 900, sc, current->getSelectedMode(), nextLv));

    } else if (current->isGameOver()) {
      PlayerScoreCard *sc = current->getScoreCard();
      const char *mode = current->getSelectedMode();
      int level = current->getSavedLevel();
      stateManager.changeState(new GameOverState(1600, 900, sc, mode, level));

    } else if (current->isGameOverRetry()) {
      const char *mode = current->getSelectedMode();
      int level = current->getSavedLevel();
      stateManager.changeState(new LoadingState(1600, 900, mode, level));

    } else if (current->isGameOverMenu()) {
      stateManager.changeState(new MenuState());

    } else if (current->isResultsDone()) {
      int nextLv = current->getSavedLevel();
      const char *mode = current->getSelectedMode();
      // Determine total levels for this mode to detect end-of-game
      int maxLevel = 4; // Campaign/Survival max
      if (areEqual(mode, "Infinity"))
        maxLevel = 1;
      if (nextLv > maxLevel) {
        // All levels done — return to main menu
        stateManager.changeState(new MenuState());
      } else {
        stateManager.changeState(new LoadingState(1600, 900, mode, nextLv));
      }
    }
  }

  void handleEvents() {
    Event e;
    while (window.pollEvent(e)) {
      if (e.type == Event::Closed)
        window.close();
    }
    if (Keyboard::isKeyPressed(Keyboard::Escape))
      window.close();

    stateManager.handleInput();

    // Pass active player so the terminal can inject into live state
    PlayerSoldier *activePlayer = nullptr;
    GameState *cur = stateManager.getCurrentState();
    if (cur)
      activePlayer = cur->getActivePlayer();
    devTerminal.checkTrigger(activePlayer);
  }

  void render() {
    GameState *current = stateManager.getCurrentState();
    if (current && !current->isMenu())
      window.clear(Color(135, 206, 235));
    else
      window.clear(Color::Black);

    stateManager.draw(window);
    window.display();
  }

public:
  void quit() { window.close(); }
  RenderWindow &getWindow() { return window; }
};