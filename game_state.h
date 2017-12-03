#pragma once

#include "player.h"
#include "map.h"

struct GameState {
  GameState(unsigned int seed);

  Player player;
  Map map;
};
