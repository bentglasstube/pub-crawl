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

    std::pair<int, int> get_position() const;
    void set_position(int x, int y);

    double drunkenness() const;
    int money() const;

    void drink(double abv, double volume);
    void spend(int amount);

  private:

    static constexpr double kWalkSpeed = 0.01;
    static constexpr double kDrunkDecay = 0.007 / 60000;
    static constexpr double kPlayerWeight = 180;
    static constexpr double kMaleRate = 0.73;
    static constexpr double kFemaleRate = 0.66;

    double x_, y_, vx_, vy_;
    double drunk_;
    int money_;
    std::default_random_engine rng_;
};
