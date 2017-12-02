#pragma once

#include "graphics.h"

#include "map.h"

class Player {
  public:

    enum class Direction { North, East, South, West };

    Player(int x, int y);

    void update(const Map& map, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    void move(Direction dir);
    void stop();

    void set_position(int x, int y);

  private:

    static constexpr double kWalkSpeed = 0.02;

    double x_, y_, vx_, vy_;
};
