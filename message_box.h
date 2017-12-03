#pragma once

#include "graphics.h"
#include "spritemap.h"
#include "text.h"

class MessageBox {
  public:

    MessageBox(int cols, int rows, const std::string& message);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    void skip();
    bool done() const;

  private:

    static constexpr double kUpdateRate = 0.02;

    SpriteMap sprites_;
    Text text_;

    int cols_, rows_;
    std::string message_;
    double progress_;
};
