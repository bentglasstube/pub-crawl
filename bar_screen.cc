#include "bar_screen.h"

#include "map_screen.h"

BarScreen::BarScreen(GameState state, const Building& pub) :
  state_(state), pub_(pub),
  backdrop_("bar.png"), text_("text.png") {}

bool BarScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  return !input.key_pressed(Input::Button::Start);
}

void BarScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);

  for (int i = 0; i < 3; ++i) {
    text_.draw(graphics, pub_.taps[i]->name, 104, 20 + 14 * i);
  }
}

Screen* BarScreen::next_screen() const {
  return new MapScreen(state_);
}

std::string BarScreen::get_music_track() const {
  return "";
}
