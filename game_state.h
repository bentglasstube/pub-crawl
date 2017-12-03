#pragma once

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

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

  private:

    static constexpr int kStartTime = 20;

    Text text_;
};
