#include "Core Engine And States/Game.h"
#include "headers/headers.h"  
#include <cstdlib>
#include <ctime>

#include "Manager/EntityManager.h"
#include "ScoreCard/ScoreCard.h"

//Define the static instances here exactly once
EntityManager* EntityManager::instance = nullptr;
ScoreCard* ScoreCard::activeInstance = nullptr;


int main() {
  srand(time(0));
  Game game;
  game.run();
  return 0;
}