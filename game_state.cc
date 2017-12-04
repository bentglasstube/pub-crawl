#include "game_state.h"

#include <iomanip>
#include <sstream>

GameState::GameState(unsigned int seed) : time(0), player(0, 0), map(seed), text_("text.png") {
  const auto start = map.start_position();
  player.set_position(start.first, start.second);
}

void GameState::update(unsigned int elapsed) {
  player.update(map, elapsed);
  time += elapsed;
}

void GameState::draw(Graphics& graphics) const {
  std::stringstream clock;

  const int hour = (kStartTime + time / 1000 / 60) % 24;
  const int minute = (time / 1000) % 60;

  clock << std::setw(2) << std::setfill('0') << hour << ":";
  clock << std::setw(2) << std::setfill('0') << minute;
  text_.draw(graphics, clock.str(), 0, 0, Text::Alignment::Left);

  std::stringstream wallet;
  wallet << "$" << std::setw(4) << player.money();
  text_.draw(graphics, wallet.str(), 0, 16, Text::Alignment::Left);

#ifndef NDEBUG
  std::stringstream bac;
  bac << std::setprecision(3) << player.drunkenness();
  text_.draw(graphics, bac.str(), 0, 32, Text::Alignment::Left);
#endif
}

bool GameState::bars_closed() const {
  return time >= kClosingTime;
}

bool GameState::past_bedtime() const {
  return time >= kBedTime;
}
