#include "player.h"

#include "util.h"

Player::Player(int x, int y) :
  x_(x), y_(y), vx_(0), vy_(0),
  drunk_(0), money_(100), rng_(Util::random_seed()) {}

void Player::update(const Map& map, unsigned int elapsed) {
  if (vx_ != 0 || vy_ != 0) {
    std::uniform_real_distribution<double> stumble(0, 1);
    if (stumble(rng_) < 2 * drunk_) {
      std::uniform_int_distribution<int> stumble_dir(0, 3);
      move(static_cast<Direction>(stumble_dir(rng_)));
    }

    const double nx = x_ + vx_ * elapsed;
    const double ny = y_ + vy_ * elapsed;

    if (map.walkable(nx, ny)) {
      x_ = nx;
      y_ = ny;
    } else {
      stop();
    }
  }

  if (drunk_ > 0) {
    drunk_ -= elapsed * kDrunkDecay;
    if (drunk_ < 0) drunk_ = 0;
  }
}

void Player::draw(Graphics& graphics) const {
  SDL_Rect r = { (int) x_, (int) y_, 1, 1 };
  graphics.draw_rect(&r, 0xffffffff, true);
}

void Player::move(Direction dir) {
  switch (dir) {
    case Direction::North:
      vx_ = 0;
      vy_ = -kWalkSpeed;
      break;

    case Direction::East:
      vx_ = kWalkSpeed;
      vy_ = 0;
      break;

    case Direction::South:
      vx_ = 0;
      vy_ = kWalkSpeed;
      break;

    case Direction::West:
      vx_ = -kWalkSpeed;
      vy_ = 0;
      break;
  }
}

void Player::stop() {
  vx_ = 0;
  vy_ = 0;
}

void Player::set_position(int x, int y) {
  x_ = x;
  y_ = y;
}

double Player::drunkenness() const {
  return drunk_;
}

void Player::drink(double abv, double volume) {
  // TODO model absorption rates
  // TODO allow player weight
  // TODO allow player gender
  drunk_ += abv * volume * 5.14 / kPlayerWeight * kMaleRate;
}
