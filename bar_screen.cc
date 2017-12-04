#include "bar_screen.h"

#include <iomanip>
#include <sstream>

#include "game_over_screen.h"
#include "map_screen.h"

BarScreen::BarScreen(GameState state, const Building& pub) :
  state_(state), pub_(pub),
  backdrop_("bar.png"), text_("text.png"), msg_(),
  phase_(Phase::Greeting), tab_(0)
{
  set_phase(Phase::Greeting);
  state_.pubs.insert(pub_.name);
}

bool BarScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (msg_) {
    msg_->update(elapsed);

    if (msg_->done()) {
      if (msg_->menu()) {
        if (input.key_pressed(Input::Button::Up)) msg_->previous();
        if (input.key_pressed(Input::Button::Down)) msg_->next();
        if (input.key_pressed(Input::Button::Select)) msg_->next();
        if (input.key_pressed(Input::Button::A)) handle_menu_choice();
        if (input.key_pressed(Input::Button::B)) {
          set_phase(phase_ == Phase::Greeting ? Phase::Paying : Phase::Greeting);
        }
      } else {
        if (input.key_pressed(Input::Button::A) || input.key_pressed(Input::Button::B)) {
          switch (phase_) {
            case Phase::Paying:
            case Phase::Drunk:
            case Phase::Dishes:
              return false;

            // do nothing
            default:
              break;
          }

          set_phase(state_.bars_closed() ? Phase::Paying : Phase::Greeting);
        }
      }
    } else {
      if (input.key_pressed(Input::Button::A)) msg_->skip();
      if (input.key_pressed(Input::Button::B)) msg_->skip();
    }
  }

  return true;
}

void BarScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
  state_.draw(graphics);
  if (msg_) msg_->draw(graphics);

  for (int i = 0; i < 3; ++i) {
    text_.draw(graphics, pub_.taps[i]->name, 104, 20 + 14 * i);
  }
}

Screen* BarScreen::next_screen() const {
  switch (phase_) {
    case Phase::Drunk:
    case Phase::Dishes:
      return new GameOverScreen(state_);

    default:
      return new MapScreen(state_);
  }
}

std::string BarScreen::get_music_track() const {
  return "";
}

std::string BarScreen::format_beer_info(const Beer* beer) {
  std::stringstream info;

  info << std::left << std::setw(20) << beer->name;
  info << std::right << std::setw(10) << ("$" + std::to_string(beer->price)) << "\n";
  info << std::setw(3) << std::setprecision(2) << (100 * beer->abv) << "% ABV";
  info << std::right << std::setw(22) << (beer->pour_size == 16 ? "Pint" : "Goblet") << "\n";

  return info.str();
}

void BarScreen::handle_menu_choice() {
  const size_t choice = msg_->choice();

  if (phase_ == Phase::Greeting) {
    if (choice == 0) {
      set_phase(Phase::BeerMenu);
    } else if (choice == 1) {
      set_phase(Phase::Ordering);
    } else if (choice == 2) {
      set_phase(Phase::Paying);
    }
  } else if (phase_ == Phase::BeerMenu) {
    beer_ = pub_.taps[choice];
    set_phase(Phase::BeerInfo);
  } else if (phase_ == Phase::BeerInfo) {
    if (choice == 0) {
      set_phase(Phase::Drinking);
    } else if (choice == 1){
      set_phase(Phase::BeerMenu);
    }
  } else if (phase_ == Phase::Ordering) {
    beer_ = pub_.taps[choice];
    set_phase(Phase::Drinking);
  }
}

void BarScreen::set_phase(Phase phase) {
  if (state_.player.barfing()) {
    phase_ = Phase::Drunk;
  } else {
    phase_ = phase;
  }

  switch (phase_) {
    case Phase::Greeting:
      if (state_.bars_closed()) {
        msg_.reset(new MessageBox(30, 5, "I'm sorry but we are closing."));
      } else {
        msg_.reset(new MessageBox(30, 5, "Welcome to " + pub_.name + ".\nHow can I help you?"));

        msg_->add_option("Beer Info");
        msg_->add_option("I'm ready to order");
        msg_->add_option("I'm ready to go");
      }

      break;

    case Phase::BeerMenu:
      msg_.reset(new MessageBox(30, 5, "Which beer do you want to know\nmore about?"));
      for (const auto beer : pub_.taps) {
        msg_->add_option(beer->name);
      }
      break;

    case Phase::BeerInfo:
      msg_.reset(new MessageBox(30, 5, format_beer_info(beer_)));
      msg_->add_option("I'll take one.");
      msg_->add_option("What else do you have?");
      break;

    case Phase::Ordering:
      msg_.reset(new MessageBox(30, 5, "What can I get for you?"));
      for (const auto beer : pub_.taps) {
        msg_->add_option(beer->name);
      }
      break;

    case Phase::Drinking:
      msg_.reset(new MessageBox(30, 5, "One " + beer_->name + ".\nComing right up!"));
      state_.update(20000);
      state_.player.drink(beer_->abv, beer_->pour_size);
      state_.beers.insert(beer_->name);
      tab_ += beer_->price;

      break;

    case Phase::Paying:
      if (tab_ > 0) {
        std::string message = "That'll be $" + std::to_string(tab_) + ", please.\n\n";

        if (state_.player.money() > tab_) {
          state_.player.spend(tab_);
        } else {
          message += "...you don't have enough cash?\nLooks like you're doing dishes.";
          phase_ = Phase::Dishes;
        }

        msg_.reset(new MessageBox(30, 5, message));
      } else {
        msg_.reset(new MessageBox(30, 5, "Come again soon!"));
      }
      break;

    case Phase::Drunk:
      msg_.reset(new MessageBox(30, 5, "Hey, are you doing okay?"));
      break;

    case Phase::Dishes:
      msg_.reset();
      break;
  }
}
