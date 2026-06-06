#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Forward-declare to avoid a heavy include in the base class
class PlayerSoldier;

using namespace sf;


class GameState {
public:
    virtual ~GameState() {}
    virtual void handleInput()              = 0;
    virtual void update(float dt)           = 0;
    virtual void draw(RenderWindow& window) = 0;

    virtual bool isMenu() const { return false; }
    virtual bool shouldStartGame() const { return false; }
    virtual const char* getSelectedMode() const { return "Campaign"; }
    virtual int getSavedLevel() const { return 1; }

    virtual bool isLoadingDone() const { return false; }
    virtual bool isLevelComplete() const { return false; }
    virtual bool isResultsDone() const { return false; }
    virtual bool isGameOver() const { return false; }
    virtual bool isGameOverRetry() const { return false; }
    virtual bool isGameOverMenu() const { return false; }
    virtual class PlayerScoreCard* getScoreCard() const { return nullptr; }

    // Returns the active player for dev-terminal injection; null in non-play states
    virtual PlayerSoldier* getActivePlayer() const { return nullptr; }
};