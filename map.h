#pragma once

#include <memory>
#include <random>

#include "graphics.h"

class Map {
  public:

    enum class CellType { Street, Block, Building };

    Map(unsigned int seed);

    void draw(Graphics& graphics) const;

    std::pair<int, int> start_position() const;
    CellType cell_type(int x, int y) const;
    bool walkable(int x, int y) const;

  private:

    static constexpr int kWidth = 256;
    static constexpr int kHeight = 240;
    static constexpr int kMinBlockSize = 64;
    static constexpr int kRoadBuffer = kMinBlockSize / 4;
    static constexpr int kRoadWidth = 4;

    class Block {
      public:

        enum class BuildingType { None, House, Pub };

        Block(int x, int y, int w, int h);

        void recursive_split(std::default_random_engine& r);
        void draw(Graphics& graphics) const;

        bool add_building(BuildingType type, std::default_random_engine& r);
        std::pair<int, int> find_house_door() const;

        CellType cell_type(int x, int y) const;

      private:

        int x_, y_, width_, height_;
        int bx_, by_, bwidth_, bheight_;
        BuildingType btype_;
        std::unique_ptr<Block> left_, right_;

        bool leaf() const;
        bool has_building() const;

        void split_horizontal(int pos);
        void split_vertical(int pos);

        static int building_color(BuildingType type);
    };

    void add_buildings(Block::BuildingType type, int count);

    std::default_random_engine rand_;
    std::unique_ptr<Block> root_;
};
