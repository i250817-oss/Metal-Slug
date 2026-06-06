#pragma once
#include "../Level and Environment System/perlin.h"
#include "../headers/string.h"
#include "GameState.h"
#include "PlayState.h"
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

// Main menu logic
class MainMenu {
public:
  static const int STATE_INTRO = 0;
  static const int STATE_LOADING = 1;
  static const int STATE_MAIN_SELECTION = 2;
  static const int STATE_NAME_INPUT = 3;
  static const int STATE_CONTINUE_INPUT = 4;
  static const int STATE_MODE_SELECT = 5;
  static const int STATE_PROGRESS_CHECK = 6;
  static const int STATE_ABOUT = 7;
  static const int STATE_WRONG_NAME = 8;
  static const int STATE_SCORECARD = 9;

private:
  int currentState;
  Font menuFont;
  Text titleText;
  Text infoText;
  Text authorText;
  Text loadingLabel;

  Texture texBackground;
  Sprite bgSprite;
  Texture texLoading;
  Sprite loadingSprite;

  Clock stateClock;
  int loadingFrame;
  float loadingTimer;

  int selectionIndex;
  char playerName[32];
  int nameLen;
  bool isNewGame;
  bool bgLoaded;
  bool firstLoadDone;

  char selectedMode[32];
  int savedLevel;

  IntRect frames[10] = {{284, 208, 3026, 328},   {284, 669, 3027, 340},
                        {287, 1145, 3023, 338},  {284, 1618, 3026, 338},
                        {284, 2088, 3025, 340},  {3602, 208, 3004, 329},
                        {3602, 670, 3004, 339},  {3602, 1145, 3004, 337},
                        {3602, 1618, 3004, 338}, {3602, 2089, 3004, 339}};

  bool keyWasPressed[Keyboard::KeyCount];
  Music aboutMusic;
  Music menuMusic;
  bool menuMusicPlaying;
  Texture texAboutSky;
  Texture texAboutGround;
  Texture texAboutPlane;
  Sprite sprAboutSky;
  Sprite sprAboutGround;
  Sprite sprAboutPlane;
  float aboutSkyX;
  float aboutGroundX;
  float aboutTextY;
  int aboutPlanePhase; // 0=forward, 1=tilt, 2=back
  int aboutPlaneFrame;
  float aboutPlaneTimer;
  float aboutPlaneX, aboutPlaneY;
  bool aboutSequenceFinished;
  float aboutTimer;

  bool isJustPressed(Keyboard::Key key) {
    bool down = Keyboard::isKeyPressed(key);
    bool result = (down && !keyWasPressed[key]);
    keyWasPressed[key] = down;
    return result;
  }

public:
  MainMenu()
      : currentState(STATE_INTRO), loadingFrame(0), loadingTimer(0),
        selectionIndex(0), nameLen(0), isNewGame(true), bgLoaded(false),
        firstLoadDone(false), savedLevel(0), menuMusicPlaying(false) {
    copyStringN(selectedMode, "Campaign", 32);
    for (int i = 0; i < Keyboard::KeyCount; i++)
      keyWasPressed[i] = false;
    for (int i = 0; i < 32; i++)
      playerName[i] = '\0';

    if (menuMusic.openFromFile("sound/menu_theme.ogg")) {
      menuMusic.setVolume(100);
      menuMusic.setLoop(true);
      menuMusic.play();
      menuMusicPlaying = true;
    }

    if (!menuFont.loadFromFile("Founts/Metal-Slug-Latino-Regular.ttf")) {
      cerr << "[Error] Failed to load font" << endl;
    }

    titleText.setFont(menuFont);
    titleText.setCharacterSize(70);
    titleText.setFillColor(Color(255, 215, 0));

    infoText.setFont(menuFont);
    infoText.setCharacterSize(35);
    infoText.setFillColor(Color::Green);

    authorText.setFont(menuFont);
    authorText.setCharacterSize(25);
    authorText.setFillColor(Color(255, 215, 0));

    loadingLabel.setFont(menuFont);
    loadingLabel.setString("LOADING");
    loadingLabel.setCharacterSize(50);
    loadingLabel.setFillColor(Color::White);

    texLoading.loadFromFile("main menu/loading ufxc.png");
    loadingSprite.setTexture(texLoading);
    loadingSprite.setScale(0.3f, 0.3f);

    stateClock.restart();
  }

  ~MainMenu() {
    menuMusic.stop();
    aboutMusic.stop();
  }

  void handleInput() {
    if (currentState == STATE_MAIN_SELECTION) {
      if (isJustPressed(Keyboard::Up))
        selectionIndex = (selectionIndex > 0) ? selectionIndex - 1 : 4;
      if (isJustPressed(Keyboard::Down))
        selectionIndex = (selectionIndex < 4) ? selectionIndex + 1 : 0;
      if (isJustPressed(Keyboard::Enter)) {
        if (selectionIndex == 0) {
          isNewGame = true;
          currentState = STATE_NAME_INPUT;
          nameLen = 0;
          playerName[0] = '\0';
          bgLoaded = false;
        } else if (selectionIndex == 1) {
          isNewGame = false;
          currentState = STATE_CONTINUE_INPUT;
          nameLen = 0;
          playerName[0] = '\0';
          bgLoaded = false;
        } else if (selectionIndex == 2) {
          currentState = STATE_SCORECARD;
          bgLoaded = false;
          stateClock.restart();
        } else if (selectionIndex == 3) {
          currentState = STATE_ABOUT;
          bgLoaded = false;
          aboutSkyX = 0;
          aboutGroundX = 0;
          aboutTextY = 900.f;
          aboutPlanePhase = 0;
          aboutPlaneFrame = 0;
          aboutPlaneTimer = 0;
          aboutPlaneX = -200.f;
          aboutPlaneY = 400.f;
          aboutSequenceFinished = false;
          aboutTimer = 0.f;
          if (menuMusicPlaying) {
            menuMusic.pause();
          }
          if (aboutMusic.openFromFile("sound/MY GOATED GAME.mp3")) {
            aboutMusic.setVolume(100);
            aboutMusic.setLoop(true);
            aboutMusic.play();
            aboutMusic.setPlayingOffset(sf::seconds(100));
          }
        } else if (selectionIndex == 4) {
        }
        stateClock.restart();
      }
    } else if (currentState == STATE_NAME_INPUT ||
               currentState == STATE_CONTINUE_INPUT) {
      for (int i = Keyboard::A; i <= Keyboard::Z; i++) {
        if (isJustPressed((Keyboard::Key)i) && nameLen < 31) {
          playerName[nameLen++] = 'A' + (i - Keyboard::A);
          playerName[nameLen] = '\0';
        }
      }
      if (isJustPressed(Keyboard::BackSpace) && nameLen > 0) {
        playerName[--nameLen] = '\0';
      }
      if (isJustPressed(Keyboard::Enter) && nameLen > 0) {
        if (currentState == STATE_NAME_INPUT) {
          savePlayerInfo();
          currentState = STATE_MODE_SELECT;
          bgLoaded = false;
        } else {
          if (checkSavedPlayer()) {
            currentState = STATE_MODE_SELECT;
            bgLoaded = false;
          } else {
            currentState = STATE_WRONG_NAME;
          }
        }
        stateClock.restart();
      }
    } else if (currentState == STATE_MODE_SELECT) {
      if (isJustPressed(Keyboard::Up))
        selectionIndex = (selectionIndex > 0) ? selectionIndex - 1 : 4;
      if (isJustPressed(Keyboard::Down))
        selectionIndex = (selectionIndex < 4) ? selectionIndex + 1 : 0;
      if (isJustPressed(Keyboard::Enter)) {
        if (selectionIndex <= 3) {
          copyStringN(selectedMode, "Campaign", 32);
          savedLevel = selectionIndex + 1;
        } else {
          copyStringN(selectedMode, "Infinity", 32);
          savedLevel = 1;
        }

        if (isNewGame) {
          currentState = STATE_LOADING;
          stateClock.restart();
        } else {
          currentState = STATE_PROGRESS_CHECK;
          bgLoaded = false;
          stateClock.restart();
        }
      }
      if (isJustPressed(Keyboard::BackSpace)) {
        currentState = STATE_MAIN_SELECTION;
        bgLoaded = false;
      }
    } else if (currentState == STATE_PROGRESS_CHECK) {
      if (isJustPressed(Keyboard::Enter)) {
        currentState = STATE_LOADING;
        stateClock.restart();
      }
      if (isJustPressed(Keyboard::BackSpace)) {
        currentState = STATE_MODE_SELECT;
        bgLoaded = false;
      }
    } else if (currentState == STATE_WRONG_NAME ||
               currentState == STATE_ABOUT || currentState == STATE_SCORECARD) {
      if (isJustPressed(Keyboard::Enter) ||
          isJustPressed(Keyboard::BackSpace)) {
        if (currentState == STATE_ABOUT) {
          aboutMusic.stop();
          if (menuMusicPlaying) {
            menuMusic.play();
          }
        }
        currentState = STATE_MAIN_SELECTION;
        bgLoaded = false;
        stateClock.restart();
      }
    } else if (currentState == STATE_INTRO) {
      if (isJustPressed(Keyboard::Enter) || isJustPressed(Keyboard::Space)) {
        currentState = STATE_LOADING;
        firstLoadDone = false;
        stateClock.restart();
      }
    }
  }

  void savePlayerInfo() {
    ofstream file("player_save.txt");
    if (file.is_open()) {
      file << playerName << "\n1\n";
      file.close();
    }
  }

  bool checkSavedPlayer() {
    ifstream file("player_save.txt");
    if (file.is_open()) {
      char savedName[32];
      file >> savedName;
      file >> savedLevel;
      file.close();
      return areEqual(playerName, savedName);
    }
    return false;
  }

  void update(float dt) {
    if (currentState == STATE_INTRO) {
      float t = stateClock.getElapsedTime().asSeconds();
      if (t > 5.5f) {
        currentState = STATE_LOADING;
        firstLoadDone = false;
        stateClock.restart();
      }
    }

    if (currentState == STATE_LOADING) {
      loadingTimer += dt;
      if (loadingTimer > 0.1f) {
        loadingFrame = (loadingFrame + 1) % 10;
        loadingTimer = 0;
      }
      if (stateClock.getElapsedTime().asSeconds() > 3) {
        if (!firstLoadDone) {
          currentState = STATE_MAIN_SELECTION;
          firstLoadDone = true;
          bgLoaded = false;
          stateClock.restart();
        }
      }
    }

    if (currentState == STATE_MAIN_SELECTION && !bgLoaded) {
      texBackground.loadFromFile("main menu/MENU 1.jpg");
      bgSprite.setTexture(texBackground, true);
      float scale = 800.0f / texBackground.getSize().y;
      bgSprite.setScale(scale, scale);
      bgSprite.setPosition(50, 50);
      bgSprite.setColor(Color::White);
      bgLoaded = true;
    }
    if (currentState == STATE_ABOUT && !bgLoaded) {
      texAboutSky.loadFromFile("Sprites/ABOUT BACKGROUND 1.png");
      texAboutGround.loadFromFile("Sprites/ABOUT BACKGROUND 1.png");
      texAboutPlane.loadFromFile("Sprites/PAPER PLAIN.png");

      sprAboutSky.setTexture(texAboutSky);
      sprAboutSky.setTextureRect(IntRect(8, 190, 964, 168));
      sprAboutSky.setScale(1600.f / 964.f, 900.f / 168.f);

      sprAboutGround.setTexture(texAboutGround);
      sprAboutGround.setTextureRect(IntRect(8, 31, 1135, 153));
      sprAboutGround.setScale(1600.f / 1135.f, 900.f / 153.f);

      sprAboutPlane.setTexture(texAboutPlane);
      sprAboutPlane.setScale(4.f, 4.f);

      bgLoaded = true;
    }

    if (currentState == STATE_ABOUT && bgLoaded) {
      if (!aboutSequenceFinished) {
        if (aboutSkyX > -800.f) {
          aboutSkyX -= 5.f * dt;
        }
        aboutGroundX -= 75.f * dt;
        if (aboutGroundX <= -1600.f)
          aboutGroundX += 1600.f;

        aboutPlaneTimer += dt;
        if (aboutPlanePhase == 0) {
          aboutPlaneX += 200.f * dt;
        } else if (aboutPlanePhase == 2) {
          aboutPlaneX -= 80.f * dt;
          aboutPlaneY += 20.f * dt;
        }

        float currentAnimDelay = (aboutPlanePhase == 2) ? 0.6f : 0.15f;
        if (aboutPlaneTimer > currentAnimDelay) {
          aboutPlaneTimer = 0;
          if (aboutPlanePhase == 0) {
            aboutPlaneFrame++;
            if (aboutPlaneFrame > 3)
              aboutPlaneFrame = 0;
            if (aboutPlaneX > 1400.f) {
              aboutPlanePhase = 1;
              aboutPlaneFrame = 0;
            }
          } else if (aboutPlanePhase == 1) {
            aboutPlaneFrame++;
            if (aboutPlaneFrame > 3) {
              aboutPlanePhase = 2;
              aboutPlaneFrame = 0;
            }
          } else if (aboutPlanePhase == 2) {
            aboutPlaneFrame++;
            if (aboutPlaneFrame > 3)
              aboutPlaneFrame = 3;
          }
        }

        aboutTextY -= 45.f * dt;
        if (aboutTextY < -1100.f) {
          aboutSequenceFinished = true;
          aboutTimer = 0.f;
        }
      } else {
        aboutTimer += dt;
      }
    }
    if (currentState == STATE_SCORECARD && !bgLoaded) {
      texBackground.loadFromFile("main menu/image6.png");
      bgSprite.setTexture(texBackground, true);
      bgSprite.setTextureRect(IntRect(16, 13, 274, 206));
      bgSprite.setScale(2, 2);
      bgLoaded = true;
    }
    if ((currentState == STATE_NAME_INPUT ||
         currentState == STATE_CONTINUE_INPUT) &&
        !bgLoaded) {
      texBackground.loadFromFile("main menu/MENU 2.jpg");
      bgSprite.setTexture(texBackground, true);
      float scale = 800.0f / texBackground.getSize().y;
      bgSprite.setScale(scale, scale);
      bgSprite.setPosition(50, 50);
      bgSprite.setColor(Color::White);
      bgLoaded = true;
    }
    if (currentState == STATE_MODE_SELECT && !bgLoaded) {
      texBackground.loadFromFile("main menu/MENU 4.jpg");
      bgSprite.setTexture(texBackground, true);
      float scale = 800.0f / texBackground.getSize().y;
      bgSprite.setScale(scale, scale);
      bgSprite.setPosition(1550 - texBackground.getSize().x * scale, 50);
      bgSprite.setColor(Color::White);
      bgLoaded = true;
    }
    if (currentState == STATE_PROGRESS_CHECK && !bgLoaded) {
      texBackground.loadFromFile("main menu/IMAGE 5.jpg");
      bgSprite.setTexture(texBackground, true);
      float scale = 800.0f / texBackground.getSize().y;
      bgSprite.setScale(scale, scale);
      bgSprite.setPosition(50, 50);
      bgSprite.setColor(Color::White);
      bgLoaded = true;
    }
  }

  void draw(RenderWindow &window) {
    if (currentState == STATE_INTRO) {
      window.clear(Color::Black);
      titleText.setString("Pakistani Metal Slug");
      titleText.setFillColor(Color(255, 215, 0));
      titleText.setPosition(400, 100);
      window.draw(titleText);

      float t = stateClock.getElapsedTime().asSeconds();
      if (t > 1.2f) {
        infoText.setString("made by");
        infoText.setFillColor(Color::White);
        infoText.setPosition(700, 300);
        window.draw(infoText);
      }
      if (t > 2.2f) {
        infoText.setString("Osaid Zahid");
        infoText.setPosition(650, 400);
        window.draw(infoText);
      }
      if (t > 3.2f) {
        infoText.setString("Ahmad ali");
        infoText.setPosition(680, 500);
        window.draw(infoText);
      }
    } else if (currentState == STATE_LOADING) {
      window.clear(Color::Black);
      loadingSprite.setTextureRect(frames[loadingFrame]);
      loadingSprite.setPosition(
          1600.0f / 2 - (frames[loadingFrame].width * 0.3f) / 2, 400.0f);
      window.draw(loadingSprite);
      loadingLabel.setPosition(1600.0f / 2 - 100.0f, 600.0f);
      window.draw(loadingLabel);
    } else if (currentState == STATE_WRONG_NAME) {
      window.clear(Color::Black);
      titleText.setString("WRONG NAME");
      titleText.setFillColor(Color::Red);
      titleText.setPosition(500, 400);
      window.draw(titleText);

      infoText.setString("CLICK ENTER TO BACK TO MAIN MENU");
      infoText.setFillColor(Color::White);
      infoText.setPosition(400, 600);
      window.draw(infoText);
    } else if (currentState == STATE_ABOUT) {
      window.clear(Color::Black);

      if (!aboutSequenceFinished) {
        sprAboutSky.setPosition(aboutSkyX, 0);
        window.draw(sprAboutSky);
        sprAboutSky.setPosition(aboutSkyX + 1600.f, 0);
        window.draw(sprAboutSky);

        sprAboutGround.setPosition(aboutGroundX, 0);
        window.draw(sprAboutGround);
        sprAboutGround.setPosition(aboutGroundX + 1600.f, 0);
        window.draw(sprAboutGround);

        IntRect pFrames[3][4] = {{{4, 5, 48, 30},
                                  {56, 8, 48, 26},
                                  {108, 10, 48, 25},
                                  {160, 17, 45, 19}},
                                 {{8, 46, 46, 17},
                                  {58, 47, 45, 19},
                                  {107, 44, 44, 22},
                                  {155, 44, 45, 23}},
                                 {{25, 73, 46, 27},
                                  {75, 75, 39, 22},
                                  {118, 75, 42, 21},
                                  {164, 81, 9, 7}}};
        sprAboutPlane.setTextureRect(pFrames[aboutPlanePhase][aboutPlaneFrame]);
        sprAboutPlane.setPosition(aboutPlaneX, aboutPlaneY);
        window.draw(sprAboutPlane);

        Text t;
        t.setFont(menuFont);
        t.setStyle(Text::Bold);

        auto drawCentered = [&](const char *str, float yOffset, bool gold,
                                int cSize) {
          t.setString(str);
          t.setCharacterSize(cSize);
          t.setFillColor(gold ? Color(255, 215, 0) : Color::White);
          FloatRect bounds = t.getLocalBounds();
          t.setPosition(800.f - bounds.width / 2.f, aboutTextY + yOffset);
          window.draw(t);
        };

        drawCentered("----------- NAME OF  STUDENTS-------------", 0, true, 40);
        drawCentered("MUHMMAD OSAID ZAHID (LEADER)", 60, false, 40);
        drawCentered("AHMAD ALI", 120, false, 40);
        drawCentered("-----------SPECIAL THANK TO OUR---------", 240, true, 40);
        drawCentered("GOATED TA'S", 300, true, 40);
        drawCentered("1) MUHMMAD IBTASSAM", 360, false, 35);
        drawCentered("2) RAJA MUHMMAD ALI", 420, false, 35);
        drawCentered("3) ABDULLAH AMIR", 480, false, 35);
        drawCentered("4) HAMDA SAHID", 540, false, 35);
        drawCentered("5) OMAR ABDULLAH", 600, false, 35);
        drawCentered("6) MUHMMAD RAYYAN", 660, false, 35);
        drawCentered("7) MAHAD AHMAD", 720, false, 35);
        drawCentered("8) MUHMMAD JIBRAN", 780, false, 35);

      } else {
        Text t;
        t.setFont(menuFont);
        t.setStyle(Text::Bold);
        auto drawCenteredFix = [&](const char *str, int size, Color c,
                                   float y) {
          t.setString(str);
          t.setCharacterSize(size);
          t.setFillColor(c);
          FloatRect bounds = t.getLocalBounds();
          t.setPosition(800.f - bounds.width / 2.f, y);
          window.draw(t);
        };

        if (aboutTimer < 2.f) {
          drawCenteredFix("AND", 60, Color::White, 400.f);
        } else if (aboutTimer < 4.f) {
          drawCenteredFix("TO THE ONE", 60, Color(255, 215, 0), 300.f);
          drawCenteredFix("WHO IS OUR INSPIRATION", 40, Color::White, 380.f);
          drawCenteredFix("OUR GOAT", 50, Color::White, 450.f);
        } else if (aboutTimer < 8.f) {
          drawCenteredFix("Shehreyar Rashid", 80, Color(255, 215, 0), 350.f);
          drawCenteredFix("(IT WAS A VERY A NICE TIME WITH YOU AS A STUDENT)",
                          30, Color::White, 500.f);
        } else {
          window.clear(Color::Black);
          drawCenteredFix("THANKS", 100, Color::White, 400.f);
        }
      }

      if (aboutSequenceFinished && aboutTimer > 8.f) {
        infoText.setString("CLICK BACKSPACE TO BACK TO MAIN MENU");
        infoText.setFillColor(Color::White);
        infoText.setPosition(50, 750);
        window.draw(infoText);
      }
    } else if (currentState == STATE_SCORECARD) {
      window.clear(Color(0, 0, 139));
      float elapsed = stateClock.getElapsedTime().asSeconds();

      float px = (perlin(1.41f, 2.71f) + 1) * 0.5f;
      float py = (perlin(3.14f, 1.61f) + 1) * 0.5f;
      if (px < 0)
        px = 0;
      if (px > 1)
        px = 1;
      if (py < 0)
        py = 0;
      if (py > 1)
        py = 1;

      bgSprite.setScale(3.5f, 3.5f);
      float boxWidth = 274 * 3.5f;
      float boxHeight = 206 * 3.5f;

      float posX = px * (1600 - boxWidth);
      float posY = py * (900 - boxHeight);
      if (posY > 100)
        posY -= 100;

      bgSprite.setPosition(posX, posY);

      int colorPhase = (int)(elapsed * 4) % 3;
      if (colorPhase == 0)
        bgSprite.setColor(Color::Yellow);
      else if (colorPhase == 1)
        bgSprite.setColor(Color(255, 105, 180));
      else
        bgSprite.setColor(Color::Green);

      window.draw(bgSprite);

      struct ScoreEntry {
        int score;
        char text[256];
      };

      ScoreEntry *entries = nullptr;
      int counter = 0;

      ifstream file("score_history.txt");
      char line[256];

      if (file.is_open()) {
        while (file.getline(line, 256)) {
          int idx = findSubstring(line, " | Score: ");
          if (idx != -1) {
            idx += 10;
            int currentScore = 0;
            while (line[idx] >= '0' && line[idx] <= '9') {
              currentScore = currentScore * 10 + (line[idx] - '0');
              idx++;
            }

            ScoreEntry *temp = new ScoreEntry[counter + 1];
            for (int i = 0; i < counter; i++) {
              temp[i] = entries[i];
            }
            temp[counter].score = currentScore;
            copyStringN(temp[counter].text, line, 256);

            if (entries != nullptr)
              delete[] entries;
            entries = temp;
            counter++;
          }
        }
        file.close();
      }

      for (int i = 0; i < counter - 1; i++) {
        for (int j = i + 1; j < counter; j++) {
          if (entries[j].score > entries[i].score) {
            ScoreEntry temp = entries[i];
            entries[i] = entries[j];
            entries[j] = temp;
          }
        }
      }

      infoText.setString("LEADERBOARD:");
      infoText.setFillColor(Color::Yellow);
      infoText.setPosition(posX + 40, posY + 40);
      window.draw(infoText);

      infoText.setCharacterSize(16);

      int displayCount = (counter < 15) ? counter : 15;
      for (int i = 0; i < displayCount; i++) {
        for (int j = 0; entries[i].text[j] != '\0'; j++) {
          if (entries[i].text[j] == '|')
            entries[i].text[j] = ' ';
        }

        infoText.setString(entries[i].text);
        infoText.setFillColor(Color::White);
        infoText.setPosition(posX + 40, posY + 100 + (i * 30));
        window.draw(infoText);
      }

      if (counter == 0) {
        infoText.setString("NO SCORE HISTORY FOUND");
        infoText.setFillColor(Color::White);
        infoText.setPosition(posX + 40, posY + 100);
        window.draw(infoText);
      }

      if (entries != nullptr)
        delete[] entries;

      infoText.setCharacterSize(35);
      infoText.setString("CLICK BACKSPACE TO BACK TO MAIN MENU");
      infoText.setFillColor(Color::White);
      infoText.setPosition(400, 800);
      window.draw(infoText);
    } else {
      window.clear(Color::Black);
      window.draw(bgSprite);
      if (currentState == STATE_MAIN_SELECTION) {
        infoText.setFillColor(Color::Green);
        const char *opts[5] = {"NEW GAME", "CONTINUE GAME", "SCORECARD",
                               "ABOUT", "EXIT"};
        for (int i = 0; i < 5; i++) {
          char buf[64];
          copyStringN(buf, (selectionIndex == i ? "> " : "  "), 64);
          char *full = concatenate(buf, opts[i]);
          infoText.setString(full);
          infoText.setPosition(1000, 400 + i * 70);
          window.draw(infoText);
          delete[] full;
        }
      } else if (currentState == STATE_NAME_INPUT ||
                 currentState == STATE_CONTINUE_INPUT) {
        infoText.setFillColor(Color::Green);
        infoText.setString(currentState == STATE_NAME_INPUT
                               ? "ENTER NAME: "
                               : "ENTER SAVED NAME: ");
        infoText.setPosition(900, 350);
        window.draw(infoText);
        infoText.setString(playerName);
        infoText.setPosition(1300, 350);
        window.draw(infoText);
      } else if (currentState == STATE_MODE_SELECT) {
        titleText.setString("SELECT MISSION");
        titleText.setFillColor(Color(255, 215, 0));
        titleText.setPosition(200, 150);
        window.draw(titleText);

        infoText.setFillColor(Color::Green);
        const char *modes[] = {"LEVEL 1", "LEVEL 2", "LEVEL 3", "FINAL BOSS",
                               "INFINITY MODE"};
        for (int i = 0; i < 5; i++) {
          char buf[64];
          copyStringN(buf, (selectionIndex == i ? "> " : "  "), 64);
          char *full = concatenate(buf, modes[i]);
          infoText.setString(full);
          infoText.setPosition(200, 300 + i * 70);
          window.draw(infoText);
          delete[] full;
        }

        infoText.setString("CLICK BACKSPACE TO BACK TO MAIN MENU");
        infoText.setFillColor(Color::White);
        infoText.setPosition(200, 650);
        window.draw(infoText);
      } else if (currentState == STATE_PROGRESS_CHECK) {
        titleText.setString("CHECK PROGRESS");
        titleText.setFillColor(Color(255, 215, 0));
        titleText.setPosition(900, 200);
        window.draw(titleText);

        infoText.setFillColor(Color::Green);
        if (savedLevel >= 1)
          infoText.setString("LEVEL 1: UNLOCKED");
        else
          infoText.setString("LEVEL 1: LOCKED");
        infoText.setPosition(900, 400);
        window.draw(infoText);

        infoText.setString("PRESS ENTER TO START");
        infoText.setPosition(900, 500);
        window.draw(infoText);

        infoText.setString("CLICK BACKSPACE TO BACK TO MAIN MENU");
        infoText.setFillColor(Color::White);
        infoText.setPosition(900, 650);
        window.draw(infoText);
      }
    }
  }

  bool shouldStartGame() const {
    return firstLoadDone && currentState == STATE_LOADING &&
           stateClock.getElapsedTime().asSeconds() > 3;
  }

  const char *getSelectedMode() const { return selectedMode; }
  int getSavedLevel() const { return savedLevel; }
};

class MenuState : public GameState {
private:
  MainMenu menu;

public:
  MenuState() : menu() {}
  ~MenuState() {}

  void handleInput() override { menu.handleInput(); }
  void update(float dt) override { menu.update(dt); }
  void draw(RenderWindow &window) override { menu.draw(window); }

  bool isMenu() const override { return true; }
  bool shouldStartGame() const override { return menu.shouldStartGame(); }
  const char *getSelectedMode() const override {
    return menu.getSelectedMode();
  }
  int getSavedLevel() const override { return menu.getSavedLevel(); }
};