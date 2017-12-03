#include "map.h"

Map::Map(unsigned int seed) : seed_(seed), rand_(), root_(), buildings_(), beers_() {
  rand_.seed(seed_);
  root_.reset(new Block(0, 0, kWidth, kHeight));
  root_->recursive_split(rand_);

  add_buildings(Building::Type::Pub, kPubCount);
  add_buildings(Building::Type::House, 1);

  for (int i = 0; i < kBeerCount; ++i) {
    beers_.push_back(Beer::generate(rand_));
  }

  // TODO better randomization
  for (int i = 0; i < kPubCount * 3; ++i) {
    buildings_[i % kPubCount].add_tap(beers_[i % kBeerCount]);
  }
}

Map::Map(const Map& map) : Map(map.seed()) {}

void Map::draw(Graphics& graphics) const {
  SDL_Rect bg = { 0, 0, kWidth, kHeight};
  graphics.draw_rect(&bg, 0x000000ff, true);
  root_->draw(graphics);

  for (const auto& b : buildings_) {
    b.draw(graphics);
  }
}

std::pair<int, int> Map::start_position() const {
  for (const auto& b : buildings_) {
    if (b.type == Building::Type::House) {
      const int xl = b.x - 1;
      const int xm = b.x + b.width / 2;
      const int xr = b.x + b.width;

      const int yt = b.y - 1;
      const int ym = b.y + b.height / 2;
      const int yb = b.y + b.height;

      if (walkable(xl, ym)) return std::make_pair(xl, ym);
      if (walkable(xr, ym)) return std::make_pair(xr, ym);
      if (walkable(xm, yt)) return std::make_pair(xm, yt);
      if (walkable(xm, yb)) return std::make_pair(xm, yb);

      return std::make_pair(xm, ym);
    }
  }

  return std::make_pair(0, 0);
}

const Building& Map::building_near(int x, int y) const {
  for (const auto& b : buildings_) {
    if (b.near(x, y)) return b;
  }
  return kNullBuilding;
}

bool Map::walkable(int x, int y) const {
  if (x < 0 || x >= kWidth) return false;
  if (y < 0 || y >= kHeight) return false;
  return root_->walkable(x, y);
}

unsigned int Map::seed() const {
  return seed_;
}

Map::Block::Block(int x, int y, int w, int h) :
  x_(x), y_(y), width_(w), height_(h),
  building_(false), left_(nullptr), right_(nullptr) {}

void Map::Block::recursive_split(std::default_random_engine& r) {
  if (leaf()) {
    bool wide = width_ > kMinBlockSize;
    bool tall = height_ > kMinBlockSize;

    // Split blocks along their longer axis
    if (width_ > 2 * height_) tall = false;
    if (height_ > 2 * width_) wide = false;

    // Randomly choose a direction to split if both are viable
    if (wide && tall) {
      std::uniform_int_distribution<int> pick(0, 1);
      if (pick(r) == 0) wide = false;
    }

    if (wide) {
      std::uniform_int_distribution<int> r2(kRoadBuffer, width_ - kRoadBuffer);
      split_horizontal(r2(r));
    } else if (tall) {
      std::uniform_int_distribution<int> r2(kRoadBuffer, height_ - kRoadBuffer);
      split_vertical(r2(r));
    }
  }

  if (!leaf()) {
    left_->recursive_split(r);
    right_->recursive_split(r);
  }
}

void Map::Block::draw(Graphics& graphics) const {
  if (leaf()) {
    SDL_Rect r = { x_, y_, width_, height_ };
    graphics.draw_rect(&r, 0x386d00ff, true);
  } else {
    left_->draw(graphics);
    right_->draw(graphics);
  }
}

Building Map::Block::add_building(Building::Type type, std::default_random_engine& r) {
  std::uniform_int_distribution<int> side(0, 1);

  if (leaf()) {
    // This block already has a building
    if (has_building()) return kNullBuilding;

    int bx, by, bwidth, bheight;
    if (side(r) == 0) {
      // place building along top or bottom
      std::uniform_int_distribution<int> width(8, 16);
      std::uniform_int_distribution<int> height(4, 8);

      bwidth = width(r);
      bheight = height(r);

      std::uniform_int_distribution<int> pos(0, width_ - bwidth);
      bx = pos(r);

      if (y_ == 0) {
        by = height_ - bheight;
      } else if (y_ + height_ == kHeight) {
        by = 0;
      } else {
        by = side(r) == 0 ? 0 : height_ - bheight;
      }
    } else {
      // place building along left or right
      std::uniform_int_distribution<int> width(4, 8);
      std::uniform_int_distribution<int> height(8, 16);

      bwidth = width(r);
      bheight = height(r);

      std::uniform_int_distribution<int> pos(0, height_ - bheight);
      by = pos(r);

      if (x_ == 0) {
        bx = width_ - bwidth;
      } else if (x_ + width_ == kWidth) {
        bx = 0;
      } else {
        bx = side(r) == 0 ? 0 : width_ - bwidth;
      }
    }

    building_ = true;
    switch (type) {
      case Building::Type::House:
        return Building(x_ + bx, y_ + by, bwidth, bheight, type, "Home");

      case Building::Type::Pub:
        return Building::generate_pub(x_ + bx, y_ + by, bwidth, bheight, r);

      default:
        return kNullBuilding;
    }

  } else {
    // pick a random child to add a building to instead
    return side(r) == 0 ? left_->add_building(type, r) : right_->add_building(type, r);
  }
}

bool Map::Block::walkable(int x, int y) const {
  if (x < x_ || x >= x_ + width_) return true;
  if (y < y_ || y >= y_ + height_) return true;

  if (leaf()) return false;
  return left_->walkable(x, y) && right_->walkable(x, y);
}

bool Map::Block::leaf() const {
  return !left_;
}

bool Map::Block::has_building() const {
  return building_;
}

void Map::Block::split_horizontal(int pos) {
  left_.reset(new Block(x_, y_, pos, height_));
  right_.reset(new Block(x_ + pos + kRoadWidth, y_, width_ - pos - kRoadWidth, height_));
}

void Map::Block::split_vertical(int pos) {
  left_.reset(new Block(x_, y_, width_, pos));
  right_.reset(new Block(x_, y_ + pos + kRoadWidth, width_, height_ - pos - kRoadWidth));
}

void Map::add_buildings(Building::Type type, int count) {
  int added = 0;
  while (added < count) {
    Building b = root_->add_building(type, rand_);
    if (b.width > 0) {
      buildings_.push_back(b);
      ++added;
    }
  }
}
