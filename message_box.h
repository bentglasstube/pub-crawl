#pragma once

#include <string>
#include <vector>

#include "graphics.h"
#include "spritemap.h"
#include "text.h"

class MessageBox {
  public:

    MessageBox(int cols, int rows, const std::string& message);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    void add_option(const std::string& choice);

    void skip();
    void previous();
    void next();

    size_t choice() const;
    bool menu() const;
    bool done() const;

  private:

    static constexpr double kUpdateRate = 0.02;

    SpriteMap sprites_;
    Text text_;

    int cols_, rows_;
    size_t choice_;
    double progress_;
    std::string message_;
    std::vector<std::string> choices_;
};
