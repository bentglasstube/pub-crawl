#include "message_box.h"

MessageBox::MessageBox(int cols, int rows, const std::string& message) :
  sprites_("ui.png", 3, 8, 8), text_("text.png"),
  cols_(cols), rows_(rows), choice_(0), progress_(0.0),
  message_(message), choices_() {}

void MessageBox::update(unsigned int elapsed) {
  if (!done()) progress_ += kUpdateRate * elapsed;
}

void MessageBox::draw(Graphics& graphics) const {
  const int x = 128 - 4 * cols_ - 8;
  const int y = 240 - 16 * rows_ - 16;

  sprites_.draw(graphics, 0, x, y);
  sprites_.draw(graphics, 6, x, y + 16 * rows_ + 8);
  for (int i = 0; i < cols_; ++i) {
    sprites_.draw(graphics, 1, x + 8 * i + 8, y);
    sprites_.draw(graphics, 7, x + 8 * i + 8, y + 16 * rows_ + 8);
  }
  sprites_.draw(graphics, 2, x + 8 * cols_ + 8, y);
  sprites_.draw(graphics, 8, x + 8 * cols_ + 8, y + 16 * rows_ + 8);

  for (int i = 0; i < rows_ * 2; ++i) {
    sprites_.draw(graphics, 3, x, y + 8 * i + 8);
    sprites_.draw(graphics, 5, x + 8 * cols_ + 8, y + 8 * i + 8);
    for (int j = 0; j < cols_; ++j) {
      sprites_.draw(graphics, 4, x + 8 * j + 8, y + 8 * i + 8);
    }
  }

  text_.draw(graphics, message_.substr(0, (int) progress_), x + 8, y + 8);

  if (done()) {
    size_t count = choices_.size();
    for (size_t i = 0; i < count; ++i) {
      const int yy = y + 16 * (rows_ - count + i) + 8;
      if (i == choice_) text_.draw(graphics, ">", x + 8, yy);
      text_.draw(graphics, choices_[i], x + 24, yy);
    }
  }
}

void MessageBox::add_option(const std::string& choice) {
  choices_.push_back(choice);
}

void MessageBox::skip() {
  progress_ = message_.length();
}

void MessageBox::previous() {
  if (choice_ > 0) --choice_;
}

void MessageBox::next() {
  if (choice_ < choices_.size() - 1) ++choice_;
}

size_t MessageBox::choice() const {
  return choice_;
}

bool MessageBox::done() const {
  return progress_ >= message_.length();
}

bool MessageBox::menu() const {
  return !choices_.empty();
}
