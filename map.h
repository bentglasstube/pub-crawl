#pragma once

#include <memory>
#include <random>
#include <vector>

#include "graphics.h"

#include "building.h"

class Map {
  public:

    Map(unsigned int seed);
    Map(const Map& map);

    void draw(Graphics& graphics) const;

    std::pair<int, int> start_position() const;
    const Building& building_near(int x, int y) const;
    bool walkable(int x, int y) const;
    unsigned int seed() const;

  private:

    static constexpr int kWidth = 256;
    static constexpr int kHeight = 240;
    static constexpr int kMinBlockSize = 64;
    static constexpr int kRoadBuffer = kMinBlockSize / 4;
    static constexpr int kRoadWidth = 4;

    class Block {
      public:

        Block(int x, int y, int w, int h);

        void recursive_split(std::default_random_engine& r);
        void draw(Graphics& graphics) const;

        Building add_building(Building::Type type, std::default_random_engine& r);

        bool walkable(int x, int y) const;

      private:

        int x_, y_, width_, height_;
        bool building_;
        std::unique_ptr<Block> left_, right_;

        bool leaf() const;
        bool has_building() const;

        void split_horizontal(int pos);
        void split_vertical(int pos);
    };

    void add_buildings(Building::Type type, int count);

    const unsigned int seed_;
    std::default_random_engine rand_;
    std::unique_ptr<Block> root_;
    std::vector<Building> buildings_;
};
