#include "player.h"

Player::Player(int x, int y) : x_(x), y_(y), vx_(0), vy_(0) {}

void Player::update(const Map& map, unsigned int elapsed) {
  const double nx = x_ + vx_ * elapsed;
  const double ny = y_ + vy_ * elapsed;

  if (map.walkable(nx, ny)) {
    x_ = nx;
    y_ = ny;
  } else {
    stop();
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
