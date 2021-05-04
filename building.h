#pragma once

#include <random>
#include <string>
#include <vector>

#include "graphics.h"

struct Beer {
  std::string name;
  double abv;
  int pour_size, price;

  static Beer generate(std::default_random_engine& r);

  static const std::vector<std::string> kNames;
  static const std::vector<std::string> kStyles;
};

class Building {
  public:

    enum class Type { None, House, Pub };

    int x, y, width, height;
    Type type;
    std::string name;
    std::vector<Beer> taps;

    Building(int x, int y, int w, int h, Type type, const std::string& name);

    void draw(Graphics& graphics) const;
    bool near(int x, int y) const;

    void add_tap(const Beer& beer);

    static int building_color(Type type);
    static Building generate_pub(int x, int y, int w, int h, std::default_random_engine& r);

  private:

    static const std::vector<std::string> kAdjectives;
    static const std::vector<std::string> kAnimals;
};

static const Building kNullBuilding(0, 0, 0, 0, Building::Type::None, "");
