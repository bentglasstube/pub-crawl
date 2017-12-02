#include "map_screen.h"

#include "util.h"

MapScreen::MapScreen() : map_(Util::random_seed()), player_(128, 120), text_("text.png") {
  auto start = map_.start_position();
  player_.set_position(start.first, start.second);
}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left)) player_.move(Player::Direction::West);
  if (input.key_pressed(Input::Button::Right)) player_.move(Player::Direction::East);
  if (input.key_pressed(Input::Button::Up)) player_.move(Player::Direction::North);
  if (input.key_pressed(Input::Button::Down)) player_.move(Player::Direction::South);
  if (input.key_pressed(Input::Button::B)) player_.stop();

  player_.update(map_, elapsed);

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  map_.draw(graphics);
  player_.draw(graphics);
}

Screen* MapScreen::next_screen() const {
  return nullptr;
}

std::string MapScreen::get_music_track() const {
  return "";
}
