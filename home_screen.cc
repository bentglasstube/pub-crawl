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

  // TODO fix assumption about number of pubs and beers
  const int pubs = state_.pubs.size();
  const int beers = state_.beers.size();
  const int percent = pubs * beers * 100 / 70;

  std::stringstream score;

  score << "Pubs visited:" << std::setw(5) << pubs << "\n";
  score << "Beers tried: " << std::setw(5) << beers << "\n";
  score << "Total score: " << std::setw(4) << percent << "%";

  text_.draw(graphics, score.str(), 112, 128, Text::Alignment::Left);
}

Screen* HomeScreen::next_screen() const {
  return new TitleScreen();
}

std::string HomeScreen::get_music_track() const {
  return "lullaby.ogg";
}
