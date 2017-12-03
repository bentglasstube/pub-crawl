#include "map_screen.h"

#include "bar_screen.h"

MapScreen::MapScreen(GameState state) : state_(state), text_("text.png") {}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  // handle directions
  if (input.key_held(Input::Button::Left)) state_.player.move(Player::Direction::West);
  else if (input.key_held(Input::Button::Right)) state_.player.move(Player::Direction::East);
  else if (input.key_held(Input::Button::Up)) state_.player.move(Player::Direction::North);
  else if (input.key_held(Input::Button::Down)) state_.player.move(Player::Direction::South);
  else state_.player.stop();

  if (input.key_pressed(Input::Button::A)) {
    state_.player.stop();
    state_.update(15000);
    state_.player.drink(0.10, 16);
  }

  state_.update(elapsed);

  return !input.key_pressed(Input::Button::Start);
}

void MapScreen::draw(Graphics& graphics) const {
  state_.map.draw(graphics);
  state_.player.draw(graphics);

  const int hour = (kStartTime + state_.time / 1000 / 60) % 24;
  const int minute = (state_.time / 1000) % 60;

  const std::string time = std::to_string(hour) + ":" + std::to_string(minute);
  const std::string bac = std::to_string(state_.player.drunkenness());

  text_.draw(graphics, time, 0, 0);
  text_.draw(graphics, bac, 256, 0, Text::Alignment::Right);
}

Screen* MapScreen::next_screen() const {
  return new BarScreen(std::move(state_));
}

std::string MapScreen::get_music_track() const {
  return "";
}
