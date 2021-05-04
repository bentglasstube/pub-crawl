#include "building.h"

const std::vector<std::string> Beer::kNames = {
  "Dirt Water", "Ex-Wife", "DFTG", "Gruber", "Choke", "Karate", "Buzz",
  "Larry", "Yeancy", "Rez", "Arma", "Kababesh", "Digshake", "CF", "Wings of",
  "Ardee", "Edge's",
};
const std::vector<std::string> Beer::kStyles = {
  "IPA", "Red", "Porter", "Stout", "Pale", "Lager", "Pils", "Brown", "Rye",
  "Saison", "Barleywine", "Maibock", "Dunkel", "Cream", "Blonde", "ESB",
  "Scotch Ale", "Wit", "Dubbel", "Tripel", "Belgian", "Cider",
};

Beer Beer::generate(std::default_random_engine& r) {
  Beer beer;

  std::uniform_int_distribution<int> name_picker(0, kNames.size() - 1);
  std::uniform_int_distribution<int> style_picker(0, kStyles.size() - 1);

  beer.name = kNames[name_picker(r)] + " " + kStyles[style_picker(r)];

  // TODO base mean ABV on style
  std::normal_distribution<double> abv_picker(0.06, 0.02);
  beer.abv = abv_picker(r);
  if (beer.abv < 0.035) beer.abv = 0.035;

  // beer pour is based on strength
  beer.pour_size = beer.abv > 0.10 ? 10 : 16;

  std::uniform_int_distribution<int> price_picker(5, 12);
  beer.price = price_picker(r);

  return beer;
}

Building::Building(int x, int y, int w, int h, Type type, const std::string& name) :
  x(x), y(y), width(w), height(h), type(type), name(name), taps() {}

void Building::draw(Graphics& graphics) const {
  SDL_Rect r = { x, y, width, height };
  graphics.draw_rect(&r, building_color(type), true);
}

bool Building::near(int px, int py) const {
  if (px < x - 1 || px > x + width) return false;
  if (py < y - 1 || py > y + height) return false;
  return true;
}

void Building::add_tap(const Beer& beer) {
  taps.push_back(beer);
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

const std::vector<std::string> Building::kAdjectives = { "Hungry", "Thirsty", "Salty", "Smelly", "Angry", "Regal", "Rotten", "Pickled" };
const std::vector<std::string> Building::kAnimals = { "Kitty", "Dog", "Beaver", "Goat", "Badger", "Toad", "Dragon", "Ostrich", "Goblin" };

Building Building::generate_pub(int x, int y, int w, int h, std::default_random_engine& r) {
  std::uniform_int_distribution<int> adj_picker(0, kAdjectives.size() - 1);
  std::uniform_int_distribution<int> animal_picker(0, kAnimals.size() - 1);
  const std::string name = "The " + kAdjectives[adj_picker(r)] + " " + kAnimals[animal_picker(r)];
  return Building(x, y, w, h, Type::Pub, name);
}
