#pragma once
#include <SFML/Audio.hpp>

class SoundManager {
private:
    struct CacheEntry {
        const char* path;
        sf::SoundBuffer buffer;
        bool loaded;
    };

    static const int MAX_BUFFERS = 32;
    static const int MAX_SOUNDS = 32;

    static CacheEntry* getBufferCache() {
        static CacheEntry cache[MAX_BUFFERS] = {};
        return cache;
    }

    static sf::Sound* getActiveSounds() {
        static sf::Sound activeSounds[MAX_SOUNDS];
        return activeSounds;
    }

    static bool compareStrings(const char* s1, const char* s2) {
        if (!s1 || !s2) return false;
        int i = 0;
        while (s1[i] != '\0' && s2[i] != '\0') {
            if (s1[i] != s2[i]) return false;
            i++;
        }
        return s1[i] == s2[i];
    }

public:
    static void playSound(const char* filePath, float volume = 35.0f) {
        if (!filePath) return;

        CacheEntry* cache = getBufferCache();
        sf::Sound* activeSounds = getActiveSounds();

        // 1. Find or load the buffer
        sf::SoundBuffer* targetBuffer = nullptr;
        int freeCacheSlot = -1;

        for (int i = 0; i < MAX_BUFFERS; i++) {
            if (cache[i].loaded) {
                if (compareStrings(cache[i].path, filePath)) {
                    targetBuffer = &cache[i].buffer;
                    break;
                }
            } else if (freeCacheSlot == -1) {
                freeCacheSlot = i;
            }
        }

        if (!targetBuffer) {
            if (freeCacheSlot == -1) {
                // Cache full
                return;
            }
            if (!cache[freeCacheSlot].buffer.loadFromFile(filePath)) {
                return; // Failed to load
            }
            cache[freeCacheSlot].path = filePath;
            cache[freeCacheSlot].loaded = true;
            targetBuffer = &cache[freeCacheSlot].buffer;
        }

        // 2. Find a stopped sound slot to play
        for (int i = 0; i < MAX_SOUNDS; i++) {
            if (activeSounds[i].getStatus() == sf::Sound::Stopped) {
                activeSounds[i].setBuffer(*targetBuffer);
                activeSounds[i].setVolume(volume);
                activeSounds[i].play();
                return;
            }
        }
    }
};
