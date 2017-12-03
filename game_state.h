#pragma once

#include "player.h"
#include "map.h"

struct GameState {
  GameState(unsigned int seed);

  unsigned int time;
  Player player;
  Map map;

  void update(unsigned int elapsed);
};
