#pragma once
#include "GameState.h"

using namespace sf;


class GameStateManager {
private:
    GameState* stateStack[10];
    int stackTop;

public:
    GameStateManager() : stackTop(-1) {
        for (int i = 0; i < 10; i++) stateStack[i] = nullptr;
    }

    ~GameStateManager() {
        while (stackTop >= 0) {
            delete stateStack[stackTop];
            stateStack[stackTop] = nullptr;
            stackTop--;
        }
    }

    void addState(GameState* state) {
        if (stackTop < 9) {
            stateStack[++stackTop] = state;
        }
    }

    void removeState() {
        if (stackTop >= 0) {
            delete stateStack[stackTop];
            stateStack[stackTop] = nullptr;
            stackTop--;
        }
    }

    void changeState(GameState* state) {
        while (stackTop >= 0) {
            delete stateStack[stackTop];
            stateStack[stackTop] = nullptr;
            stackTop--;
        }
        addState(state);
    }

    GameState* getCurrentState() const {
        return (stackTop >= 0) ? stateStack[stackTop] : nullptr;
    }

    bool isEmpty() const { return stackTop < 0; }

    void handleInput()               { if (!isEmpty()) getCurrentState()->handleInput();    }
    void update(float dt)            { if (!isEmpty()) getCurrentState()->update(dt);      }
    void draw(RenderWindow& window)  { if (!isEmpty()) getCurrentState()->draw(window);    }
};