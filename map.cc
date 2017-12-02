#include "map.h"

#include <stack>

Map::Map(unsigned int seed) {
  rand_.seed(seed);
  root_.reset(new Block(0, 0, kWidth, kHeight));
  root_->recursive_split(rand_);

  add_buildings(Block::BuildingType::House, 1);
  add_buildings(Block::BuildingType::Pub, 5);
}

void Map::draw(Graphics& graphics) const {
  SDL_Rect bg = { 0, 0, kWidth, kHeight};
  graphics.draw_rect(&bg, 0x000000ff, true);
  root_->draw(graphics);
}

std::pair<int, int> Map::start_position() const {
  return root_->find_house_door();
}

Map::CellType Map::cell_type(int x, int y) const {
  if (x < 0 || x >= kWidth) return CellType::Block;
  if (y < 0 || y >= kHeight) return CellType::Block;
  return root_->cell_type(x, y);
}

bool Map::walkable(int x, int y) const {
  const auto c = cell_type(x, y);
  return c != CellType::Block;
}

Map::Block::Block(int x, int y, int w, int h) :
  x_(x), y_(y), width_(w), height_(h),
  bx_(0), by_(0), bwidth_(0), bheight_(0),
  btype_(BuildingType::None),
  left_(nullptr), right_(nullptr) {}

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

    if (has_building()) {
      SDL_Rect p = { x_ + bx_, y_ + by_, bwidth_, bheight_ };
      graphics.draw_rect(&p, building_color(btype_), true);
    }

  } else {
    left_->draw(graphics);
    right_->draw(graphics);
  }
}

bool Map::Block::add_building(BuildingType type, std::default_random_engine& r) {
  std::uniform_int_distribution<int> side(0, 1);

  if (leaf()) {
    // This block already has a building
    if (has_building()) return false;

    if (side(r) == 0) {
      // place building along top or bottom
      std::uniform_int_distribution<int> width(8, 16);
      std::uniform_int_distribution<int> height(4, 8);

      bwidth_ = width(r);
      bheight_ = height(r);

      std::uniform_int_distribution<int> pos(0, width_ - bwidth_);
      bx_ = pos(r);

      if (y_ == 0) {
        by_ = height_ - bheight_;
      } else if (y_ + height_ == kHeight) {
        by_ = 0;
      } else {
        by_ = side(r) == 0 ? 0 : height_ - bheight_;
      }
    } else {
      // place building along left or right
      std::uniform_int_distribution<int> width(4, 8);
      std::uniform_int_distribution<int> height(8, 16);

      bwidth_ = width(r);
      bheight_ = height(r);

      std::uniform_int_distribution<int> pos(0, height_ - bheight_);
      by_ = pos(r);

      if (x_ == 0) {
        bx_ = width_ - bwidth_;
      } else if (x_ + width_ == kWidth) {
        bx_ = 0;
      } else {
        bx_ = side(r) == 0 ? 0 : width_ - bwidth_;
      }
    }

    btype_ = type;
    return true;
  } else {
    // pick a random child to add a building to instead
    return side(r) == 0 ? left_->add_building(type, r) : right_->add_building(type, r);
  }
}

std::pair<int, int> Map::Block::find_house_door() const {
  if (leaf()) {
    if (btype_ == BuildingType::House) {
      const int xm = x_ + bx_ + bwidth_ / 2;
      const int ym = y_ + by_ + bheight_ / 2;

      if (bx_ == 0) {
        return std::make_pair(x_ - 1, ym);
      } else if (by_ == 0) {
        return std::make_pair(xm, y_ - 1);
      } else if (bx_ + bwidth_ == width_) {
        return std::make_pair(x_ + width_, ym);
      } else {
        return std::make_pair(xm, y_ + height_);
      }
    } else {
      return std::make_pair(0, 0);
    }
  } else {
    const auto p = left_->find_house_door();
    if (p.first > 0 || p.second > 0) return p;
    return right_->find_house_door();
  }
}

Map::CellType Map::Block::cell_type(int x, int y) const {
  if (x < x_ || x >= x_ + width_) return CellType::Street;
  if (y < y_ || y >= y_ + height_) return CellType::Street;

  if (leaf()) {
    if (has_building()) {
      if (x < x_ + bx_ || x >= x_ + bx_ + bwidth_) return CellType::Block;
      if (y < y_ + by_ || y >= y_ + by_ + bheight_) return CellType::Block;
      return CellType::Building;
    }
    return CellType::Block;
  } else {
    // TODO optimize if needed since these are sorted
    const auto c = left_->cell_type(x, y);
    return c == CellType::Street ? right_->cell_type(x, y) : c;
  }
}

bool Map::Block::leaf() const {
  return !left_;
}

bool Map::Block::has_building() const {
  return btype_ != BuildingType::None;
}

void Map::Block::split_horizontal(int pos) {
  left_.reset(new Block(x_, y_, pos, height_));
  right_.reset(new Block(x_ + pos + kRoadWidth, y_, width_ - pos - kRoadWidth, height_));
}

void Map::Block::split_vertical(int pos) {
  left_.reset(new Block(x_, y_, width_, pos));
  right_.reset(new Block(x_, y_ + pos + kRoadWidth, width_, height_ - pos - kRoadWidth));
}

int Map::Block::building_color(BuildingType type) {
  switch (type) {
    case BuildingType::Pub:
      return 0x794100ff;

    case BuildingType::House:
      return 0xdb41c3ff;

    default:
      return 0xaaaaaaff;
  }
}

void Map::add_buildings(Block::BuildingType type, int count) {
  int added = 0;
  while (added < count) {
    if (root_->add_building(type, rand_)) ++added;
  }
}
