#pragma once

#include <string>
#include <unordered_set>

#include "graphics.h"
#include "text.h"

#include "player.h"
#include "map.h"

class GameState {
  public:
    GameState(unsigned int seed);

    unsigned int time;
    Player player;
    Map map;
    std::unordered_set<std::string> beers, pubs;

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    bool bars_closed() const;
    bool past_bedtime() const;

  private:

    static constexpr int kStartTime = 20;
    static constexpr int kClosingTime = (26 - kStartTime) * 60000;
    static constexpr int kBedTime = (24 - kStartTime) * 60000;

    Text text_;
};
