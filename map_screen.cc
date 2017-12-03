#include "map_screen.h"

#include "bar_screen.h"

MapScreen::MapScreen(GameState state) : state_(state), text_("text.png") {}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left)) state_.player.move(Player::Direction::West);
  if (input.key_pressed(Input::Button::Right)) state_.player.move(Player::Direction::East);
  if (input.key_pressed(Input::Button::Up)) state_.player.move(Player::Direction::North);
  if (input.key_pressed(Input::Button::Down)) state_.player.move(Player::Direction::South);
  if (input.key_pressed(Input::Button::B)) state_.player.stop();

  state_.player.update(state_.map, elapsed);

  return !input.key_pressed(Input::Button::Start);
}

void MapScreen::draw(Graphics& graphics) const {
  state_.map.draw(graphics);
  state_.player.draw(graphics);
}

Screen* MapScreen::next_screen() const {
  return new BarScreen(std::move(state_));
}

std::string MapScreen::get_music_track() const {
  return "";
}
