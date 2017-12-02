#include "map_screen.h"

#include "util.h"

MapScreen::MapScreen() : map_(Util::random_seed()), state_(), text_("text.png") {
  auto start = map_.start_position();
  state_.player.set_position(start.first, start.second);
}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left)) state_.player.move(Player::Direction::West);
  if (input.key_pressed(Input::Button::Right)) state_.player.move(Player::Direction::East);
  if (input.key_pressed(Input::Button::Up)) state_.player.move(Player::Direction::North);
  if (input.key_pressed(Input::Button::Down)) state_.player.move(Player::Direction::South);
  if (input.key_pressed(Input::Button::B)) state_.player.stop();

  state_.player.update(map_, elapsed);

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  map_.draw(graphics);
  state_.player.draw(graphics);
}

Screen* MapScreen::next_screen() const {
  return nullptr;
}

std::string MapScreen::get_music_track() const {
  return "";
}
