#include "game_state.h"

GameState::GameState(unsigned int seed) : player(0, 0), map(seed) {
  const auto start = map.start_position();
  player.set_position(start.first, start.second);
}
