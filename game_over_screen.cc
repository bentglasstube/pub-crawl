#include "game_over_screen.h"

#include "title_screen.h"

GameOverScreen::GameOverScreen(GameState state) :
  bathroom_("bathroom.png"), kitchen_("dishes.png"),
  text_("text.png"), state_(state) {}

bool GameOverScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (!audio.music_playing()) audio.play_music("gameover.ogg");

  if (input.any_pressed()) {
    audio.stop_music();
    return false;
  }

  return true;
}

void GameOverScreen::draw(Graphics& graphics) const {
  std::string message = "";

  if (state_.player.barfing()) {
    bathroom_.draw(graphics);
    message = "You drank too much and threw up.";
  } else {
    kitchen_.draw(graphics);
    message = "You are stuck doing dishes.";
  }

  text_.draw(graphics, message, 128, 200, Text::Alignment::Center);
}

Screen* GameOverScreen::next_screen() const {
  return new TitleScreen();
}
