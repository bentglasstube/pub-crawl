#include "home_screen.h"

#include <iomanip>
#include <sstream>

#include "title_screen.h"

HomeScreen::HomeScreen(GameState state) : backdrop_("bedroom.png"), text_("text.png"), state_(std::move(state)) {}

bool HomeScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  return !input.any_pressed();
}

void HomeScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);

  const int pubs = state_.map.pubs_visited();
  const int beers = state_.map.beers_drank();

  // TODO constants!
  const double percent = pubs * beers / 70.0;

  std::stringstream score;

  score << "Pubs visited:" << std::setw(4) << pubs << " /  7\n";
  score << "Beers tried: " << std::setw(4) << beers << " / 10\n";
  score << "Total score: " << std::setw(8) << std::setprecision(3) << (100 * percent) << "%";

  text_.draw(graphics, score.str(), 112, 128, Text::Alignment::Left);
}

Screen* HomeScreen::next_screen() const {
  return new TitleScreen();
}

std::string HomeScreen::get_music_track() const {
  return "";
}
