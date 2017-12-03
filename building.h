#pragma once

#include <random>
#include <string>
#include <vector>

#include "graphics.h"

class Building {
  public:

    enum class Type { None, House, Pub };

    int x, y, width, height;
    Type type;
    std::string name;

    Building(int x, int y, int w, int h, Type type, const std::string& name);
    void draw(Graphics& graphics) const;
    bool near(int x, int y) const;

    static int building_color(Type type);
};

class House : public Building {
  public:

    House(int x, int y, int w, int h);

};


class Pub : public Building {
  public:

    struct Beer {
      std::string name;
      double abv;
      int pour_size, price;
    };

    Pub(int x, int y, int w, int h, std::default_random_engine& r);

    void add_beer(const Beer);
    Beer* get_beer(int index) const;

    static Beer generate_beer(std::default_random_engine r);

  private:

    static std::string generate_name(std::default_random_engine& r);
    static const std::vector<std::string> kAdjectives;
    static const std::vector<std::string> kAnimals;

    std::vector<Beer*> taps_;
    bool visited_;
};

static const Building kNullBuilding(0, 0, 0, 0, Building::Type::None, "");

