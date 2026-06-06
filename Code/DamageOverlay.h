#pragma once
#include "../headers/headers.h"

using namespace sf;

// Damage overlay effect
class DamageOverlay {
private:
    int   state;      // 0=healthy, 1=injured, 2=critical
    float stateTimer; // timer
    bool  justDied;

    static const float STATE_DURATION;

public:
    DamageOverlay()
        : state(0), stateTimer(0), justDied(false)
    {}

    bool onHit() {
        justDied = false;
        if (state == 0) {
            state      = 1;
            stateTimer = STATE_DURATION;
            return false;
        }
        if (state == 1) {
            state      = 2;
            stateTimer = STATE_DURATION;
            return false;
        }
        if (state == 2) {
            justDied = true;
            return true;
        }
        return false;
    }

    void heal(int stages = 1) {
        state -= stages;
        if (state < 0) state = 0;
        if (state == 0) stateTimer = 0;
        else stateTimer = STATE_DURATION;
    }

    void update(float dt) {
        justDied = false;
        if (state == 0) return;
        stateTimer -= dt;
        if (stateTimer <= 0) {
            state      = 0;
            stateTimer = 0;
        }
    }

    void draw(RenderWindow& window, float sw, float sh) {
        if (state == 0) return;

        Uint8 alpha;
        Color fill;
        float ratio = (stateTimer / STATE_DURATION);

        if (state == 1) {
            alpha = (Uint8)(60 + ratio * 30);
            fill  = Color(200, 20, 20, alpha);
        } else {
            alpha = (Uint8)(100 + ratio * 60);
            fill  = Color(180, 0, 0, alpha);
        }

        const float edgeW = sw * 0.25f;
        const float edgeH = sh * 0.25f;

        RectangleShape top(Vector2f(sw, edgeH));
        top.setPosition(0, 0);
        top.setFillColor(fill);
        window.draw(top);

        RectangleShape bot(Vector2f(sw, edgeH));
        bot.setPosition(0, sh - edgeH);
        bot.setFillColor(fill);
        window.draw(bot);

        RectangleShape left(Vector2f(edgeW, sh));
        left.setPosition(0, 0);
        left.setFillColor(fill);
        window.draw(left);

        RectangleShape right(Vector2f(edgeW, sh));
        right.setPosition(sw - edgeW, 0);
        right.setFillColor(fill);
        window.draw(right);
    }

    int  getState()    const { return state; }
    bool getJustDied() const { return justDied; }
    void reset()             { state = 0; stateTimer = 0; justDied = false; }
};

