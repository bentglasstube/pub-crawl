#include "map_screen.h"

#include "bar_screen.h"
#include "home_screen.h"

MapScreen::MapScreen(GameState state) : state_(state), text_("text.png"), msg_() {}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (msg_) {
    msg_->update(elapsed);

    const bool mash = input.key_pressed(Input::Button::A) || input.key_pressed(Input::Button::B);
    if (mash) {
      if (msg_->done()) {
        msg_.reset();
      } else {
        msg_->skip();
      }
    }
  } else {
    // handle directions
    if (input.key_held(Input::Button::Left)) state_.player.move(Player::Direction::West);
    else if (input.key_held(Input::Button::Right)) state_.player.move(Player::Direction::East);
    else if (input.key_held(Input::Button::Up)) state_.player.move(Player::Direction::North);
    else if (input.key_held(Input::Button::Down)) state_.player.move(Player::Direction::South);
    else state_.player.stop();

    state_.update(elapsed);

    if (input.key_pressed(Input::Button::A)) {
      const auto p = state_.player.get_position();
      const Building& b = state_.map.building_near(p.first, p.second);

      if (b.type == Building::Type::Pub) {
        if (state_.bars_closed()) {
          msg_.reset(new MessageBox(30, 1, "Looks like they're closed."));
        } else {
          return false;
        }
      }
      if (b.type == Building::Type::House) {
        if (state_.past_bedtime()) {
          return false;
        } else {
          msg_.reset(new MessageBox(30, 1, "You aren't ready for bed yet."));
        }
      }
    }

    if (input.key_pressed(Input::Button::Select)) {
      state_.player.stop();
      state_.update(15000);
      state_.player.drink(0.10, 16);
    }
  }

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  state_.map.draw(graphics);
  state_.player.draw(graphics);

  const auto p = state_.player.get_position();
  const Building& b = state_.map.building_near(p.first, p.second);
  if (b.width > 0) {
    const int ty = state_.player.get_position().second > 120 ? 16 : 208;
    text_.draw(graphics, b.name, 128, ty, Text::Alignment::Center);
  }

  state_.draw(graphics);

  if (msg_) msg_->draw(graphics);
}

Screen* MapScreen::next_screen() const {
  const auto p = state_.player.get_position();
  const Building& b = state_.map.building_near(p.first, p.second);
  if (b.width > 0) {
    if (b.type == Building::Type::Pub) {
      return new BarScreen(state_, b);
    } else {
      return new HomeScreen(state_);
    }
  }

  return nullptr;
}

std::string MapScreen::get_music_track() const {
  return "";
}
