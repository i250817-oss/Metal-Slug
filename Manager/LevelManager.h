#pragma once
#include "../Level and Environment System/Level.h"



//levelManager
//ported from Ver3 — dynamic array of Level*, doubles capacity on resize.
class LevelManager {
private:
    Level** levels;
    int     count;
    int     capacity;
    int     currentIndex;

    void resize() {
        int newCap    = capacity * 2;
        Level** newArr = new Level*[newCap];
        for (int i = 0; i < count;  i++) newArr[i] = levels[i];
        for (int i = count; i < newCap; i++) newArr[i] = nullptr;
        delete[] levels;
        levels   = newArr;
        capacity = newCap;
    }

public:
    LevelManager()
        : levels(nullptr), count(0), capacity(4), currentIndex(0)
    {
        levels = new Level*[capacity];
        for (int i = 0; i < capacity; i++) levels[i] = nullptr;
    }

    ~LevelManager() {
        for (int i = 0; i < count; i++) delete levels[i];
        delete[] levels;
        levels = nullptr;
    }

    void addLevel(Level* lv) {
        if (count >= capacity) resize();
        levels[count++] = lv;
    }

    Level* getCurrentLevel() const {
        return (currentIndex >= 0 && currentIndex < count) ? levels[currentIndex] : nullptr;
    }

    void nextLevel() {
        if (currentIndex < count - 1) currentIndex++;
    }

    bool allLevelsComplete() const { return currentIndex >= count - 1; }
    int  getCurrentLevelIndex() const { return currentIndex; }
    int  getLevelCount()        const { return count; }
};