#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include "../Character System/PlayerSoldier.h"

using namespace std;
using namespace sf;


// ─── Urdu Assembly Injection REPL ────────────────────────────────────────────
class UrduAssemblyInjector {
public:
    void runREPL(PlayerSoldier* p) {
        cout << "\033[2J\033[H"; // clear screen
        printHUD(p);
        cout << "\033[90m(KHATAM to exit  |  MADAD for help)\033[0m\n\n";

        string line;
        while (true) {
            cout << "\033[92m> \033[0m" << flush;
            if (!getline(cin, line)) break;

            // trim
            size_t s = line.find_first_not_of(" \t\r\n");
            if (s == string::npos) continue;
            line = line.substr(s, line.find_last_not_of(" \t\r\n") - s + 1);
            if (line.empty()) continue;

            string up = toUpper(line);
            if (up == "KHATAM" || up == "EXIT" || up == "QUIT") break;
            if (up == "MADAD" || up == "HELP" || up == "?") { printHelp(); continue; }
            if (up == "STATUS" || up == "HAAL")              { printHUD(p); continue; }

            execute(line, p);
            printHUD(p);   // refresh HUD after every command
        }
    }

private:
    // ── HUD ─────────────────────────────────────────────────────────────────
    void printHUD(PlayerSoldier* p) {
        if (!p) { cout << "\033[31mno player\033[0m\n"; return; }

        int hp      = p->getHp();
        int maxHp   = p->getMaxHp();
        int lives   = p->getLives();
        int gren    = p->getGrenadeCount();
        int score   = p->getScores();

        cout << "\033[90m────────────────────────────\033[0m\n";
        cout << " \033[91m♥ \033[0m" << hpBar(hp, maxHp)
             << "  " << hp << "/" << maxHp << "\n";
        cout << " \033[93m★ \033[0mLIVES    " << lives    << "\n";
        cout << " \033[96m✦ \033[0mGRENADES " << gren     << "\n";
        cout << " \033[95m● \033[0mSCORE    " << score    << "\n";
        cout << "\033[90m────────────────────────────\033[0m\n";
    }

    string hpBar(int hp, int maxHp) {
        const int W = 10;
        int filled = (maxHp > 0) ? (hp * W / maxHp) : 0;
        string bar = "[";
        for (int i = 0; i < W; i++)
            bar += (i < filled) ? "\033[91m█\033[0m" : "\033[90m░\033[0m";
        bar += "]";
        return bar;
    }

    // ── Instruction parsing ──────────────────────────────────────────────────
    void execute(const string& line, PlayerSoldier* p) {
        string mn, var, valStr;
        istringstream ss(line.substr(0, line.find("//")));
        string tok; int idx = 0;
        while (ss >> tok) {
            if (!tok.empty() && tok.back() == ',') tok.pop_back();
            if (idx == 0) mn = tok;
            else if (idx == 1) var = tok;
            else if (idx == 2) valStr = tok;
            idx++;
        }

        string m = toUpper(mn);
        if (m == "AARAM" || m == "NOP") return;

        if (var.empty() || valStr.empty()) {
            cout << "\033[31merr: <MNEMONIC> <VAR>, <N>\033[0m\n"; return;
        }

        int val = 0;
        try { val = stoi(valStr); }
        catch (...) { cout << "\033[31merr: bad value\033[0m\n"; return; }

        string v = toUpper(var);
        if      (m == "JAMA"    || m == "ADD") delta(p, v, +val);
        else if (m == "TAFREEK" || m == "SUB") delta(p, v, -val);
        else if (m == "BHARO"   || m == "DAALO" || m == "SET") absolute(p, v, val);
        else cout << "\033[31merr: unknown '" << mn << "'\033[0m\n";
    }

    void delta(PlayerSoldier* p, const string& v, int d) {
        if (!p) return;
        if      (v == "HP" || v == "HEALTH") p->setHp(clamp(p->getHp() + d, 0, p->getMaxHp()));
        else if (v == "LIVES")               p->setLives(clamp(p->getLives() + d, 0, 99));
        else if (v == "GRENADES")            p->setGrenadeCount(clamp(p->getGrenadeCount() + d, 0, 999));
        else if (v == "SCORE")               p->setScores(max(0, p->getScores() + d));
        else cout << "\033[31merr: unknown var '" << v << "'\033[0m\n";
    }

    void absolute(PlayerSoldier* p, const string& v, int val) {
        if (!p) return;
        if      (v == "HP" || v == "HEALTH") p->setHp(clamp(val, 0, p->getMaxHp()));
        else if (v == "LIVES")               p->setLives(clamp(val, 0, 99));
        else if (v == "GRENADES")            p->setGrenadeCount(clamp(val, 0, 999));
        else if (v == "SCORE")               p->setScores(max(0, val));
        else cout << "\033[31merr: unknown var '" << v << "'\033[0m\n";
    }

    void printHelp() {
        cout << "\033[90m JAMA/TAFREEK/BHARO/DAALO  <HP|LIVES|GRENADES|SCORE>, <N>\033[0m\n";
    }

    string toUpper(const string& s) const {
        string r = s;
        for (char& c : r) if (c >= 'a' && c <= 'z') c = c - 32;
        return r;
    }
    int clamp(int v, int lo, int hi) const { return v < lo ? lo : v > hi ? hi : v; }
    int max(int a, int b)            const { return a > b ? a : b; }
};

// ─── Trigger + shell ─────────────────────────────────────────────────────────
class UrduDeveloperTerminal {
    int   pressCount  = 0;
    bool  wasPressed  = false;
    Clock timer;
    UrduAssemblyInjector injector;

public:
    void checkTrigger(PlayerSoldier* activePlayer = nullptr) {
        bool down = Keyboard::isKeyPressed(Keyboard::B);
        if (down && !wasPressed) {
            if (timer.getElapsedTime().asSeconds() > 2.0f) pressCount = 0;
            if (pressCount == 0) timer.restart();
            if (++pressCount >= 3) {
                pressCount = 0;
                injector.runREPL(activePlayer);
            }
        }
        wasPressed = down;
    }
};
