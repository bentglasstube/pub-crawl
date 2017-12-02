#include "map.h"

Map::Map(unsigned int seed) {
  rand_.seed(seed);
  root_.reset(new Block(0, 0, kWidth, kHeight));
  root_->recursive_split(rand_);

  add_buildings(Building::Type::House, 1);
  add_buildings(Building::Type::Pub, 5);
}

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

      if (cell_type(xl, ym) == CellType::Street) return std::make_pair(xl, ym);
      if (cell_type(xr, ym) == CellType::Street) return std::make_pair(xr, ym);
      if (cell_type(xm, yt) == CellType::Street) return std::make_pair(xm, yt);
      if (cell_type(xm, yb) == CellType::Street) return std::make_pair(xm, yb);

      return std::make_pair(xm, ym);
    }
  }

  return std::make_pair(0, 0);
}

Map::CellType Map::cell_type(int x, int y) const {
  if (x < 0 || x >= kWidth) return CellType::Block;
  if (y < 0 || y >= kHeight) return CellType::Block;

  for (const auto& b : buildings_) {
    if (b.contains(x, y)) return CellType::Building;
  }

  return root_->cell_type(x, y);
}

bool Map::walkable(int x, int y) const {
  const auto c = cell_type(x, y);
  return c != CellType::Block;
}

Map::Building::Building(int x, int y, int w, int h, Type type) :
  x(x), y(y), width(w), height(h), type(type) {}

void Map::Building::draw(Graphics& graphics) const {
  SDL_Rect r = { x, y, width, height };
  graphics.draw_rect(&r, building_color(type), true);
}

bool Map::Building::contains(int px, int py) const {
  if (px < x || px >= x + width) return false;
  if (py < y || py >= y + height) return false;
  return true;
}

int Map::Building::building_color(Type type) {
  switch (type) {
    case Type::Pub:
      return 0x794100ff;

    case Type::House:
      return 0xdb41c3ff;

    default:
      return 0xaaaaaaff;
  }
}

const Map::Building Map::kNullBuilding(0, 0, 0, 0, Map::Building::Type::None);

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

Map::Building Map::Block::add_building(Building::Type type, std::default_random_engine& r) {
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
    return std::move(Building(x_ + bx, y_ + by, bwidth, bheight, type));
  } else {
    // pick a random child to add a building to instead
    return std::move(side(r) == 0 ? left_->add_building(type, r) : right_->add_building(type, r));
  }
}

Map::CellType Map::Block::cell_type(int x, int y) const {
  if (x < x_ || x >= x_ + width_) return CellType::Street;
  if (y < y_ || y >= y_ + height_) return CellType::Street;

  if (leaf()) {
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
    const Building b = std::move(root_->add_building(type, rand_));
    if (b.width > 0) {
      buildings_.push_back(std::move(b));
      ++added;
    }
  }
}
