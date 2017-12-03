#include "bar_screen.h"

#include "map_screen.h"

BarScreen::BarScreen(GameState state) : state_(state), backdrop_("bar.png"), text_("text.png") {}

bool BarScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  return !input.key_pressed(Input::Button::Start);
}

void BarScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);

  text_.draw(graphics, "Dirt Water IPA  $10", 104, 20);
  text_.draw(graphics, "Diggenbrau      $15", 104, 34);
  text_.draw(graphics, "Ex-Wife Porter  $ 6", 104, 48);
}

Screen* BarScreen::next_screen() const {
  return new MapScreen(state_);
}

std::string BarScreen::get_music_track() const {
  return "";
}
