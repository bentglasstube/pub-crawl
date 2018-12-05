#include "title_screen.h"

#include "util.h"

#include "game_state.h"
#include "map_screen.h"

TitleScreen::TitleScreen() : backdrop_("title.png"), text_("text.png"), timer_(0) {}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (!audio.music_playing()) audio.play_music("title.ogg");
  timer_ = (timer_ + elapsed) % 1000;
  if (input.any_pressed()) {
    audio.stop_music();
    return false;
  }
  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);

  if (timer_ < 500) text_.draw(graphics, "Press any key", 128, 220, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  GameState state(Util::random_seed());
  return new MapScreen(state);
}
