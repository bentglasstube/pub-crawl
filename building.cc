#include "building.h"

Beer Beer::generate(std::default_random_engine r) {
  Beer beer;

  // TODO randomize
  beer.name = "Dirt Water IPA";
  beer.abv = 0.065;
  beer.pour_size = 12;
  beer.price = 8;

  return beer;
}

Building::Building(int x, int y, int w, int h, Type type, const std::string& name) :
  x(x), y(y), width(w), height(h), type(type), name(name), visited(false) {}

void Building::draw(Graphics& graphics) const {
  SDL_Rect r = { x, y, width, height };
  graphics.draw_rect(&r, building_color(type), true);
}

bool Building::near(int px, int py) const {
  if (px < x - 1 || px > x + width) return false;
  if (py < y - 1 || py > y + height) return false;
  return true;
}

void Building::add_tap(Beer& beer) {
  taps.push_back(&beer);
}

int Building::building_color(Type type) {
  switch (type) {
    case Type::Pub:
      return 0xebd320ff;
    case Type::House:
      return 0xdb41c3ff;
    default:
      return 0xaaaaaaff;
  }
}

House::House(int x, int y, int w, int h) : Building(x, y, w, h, Type::House, "Home") {}

Pub::Pub(int x, int y, int w, int h, std::default_random_engine& r) :
  Building(x, y, w, h, Type::Pub, Pub::generate_name(r)) {}

std::string Pub::generate_name(std::default_random_engine& r) {
  std::uniform_int_distribution<int> adj_picker(0, kAdjectives.size() - 1);
  std::uniform_int_distribution<int> animal_picker(0, kAnimals.size() - 1);
  return "The " + kAdjectives[adj_picker(r)] + " " + kAnimals[animal_picker(r)];
}

const std::vector<std::string> Pub::kAdjectives = { "Hungry", "Thirsty", "Salty", "Smelly", "Angry", "Regal", "Rotten", "Pickled" };
const std::vector<std::string> Pub::kAnimals = { "Kitty", "Dog", "Beaver", "Goat", "Badger", "Toad", "Dragon", "Ostrich", "Goblin" };
