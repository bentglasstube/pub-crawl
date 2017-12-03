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

  state_.update(elapsed);

  if (input.key_pressed(Input::Button::A)) {
    const auto p = state_.player.get_position();
    const Map::Building& b = state_.map.building_near(p.first, p.second);
    if (b.width > 0) return false;
  }

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  state_.map.draw(graphics);
  state_.player.draw(graphics);

  const int hour = (kStartTime + state_.time / 1000 / 60) % 24;
  const int minute = (state_.time / 1000) % 60;

  const std::string time = std::to_string(hour) + ":" + std::to_string(minute);
  text_.draw(graphics, time, 0, 0);

#ifndef NDEBUG
  const std::string bac = std::to_string(state_.player.drunkenness());
  text_.draw(graphics, bac, 256, 0, Text::Alignment::Right);
#endif

  const auto p = state_.player.get_position();
  const Map::Building& b = state_.map.building_near(p.first, p.second);
  if (b.width > 0) {
    // TODO show name instead of generic text
    const int ty = state_.player.get_position().second > 120 ? 16 : 208;
    text_.draw(graphics, b.name(), 128, ty, Text::Alignment::Center);
  }
}

Screen* MapScreen::next_screen() const {
  return new BarScreen(std::move(state_));
}

std::string MapScreen::get_music_track() const {
  return "";
}
