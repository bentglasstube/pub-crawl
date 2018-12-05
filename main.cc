#include "game.h"

#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "ld40";
  config.graphics.width = 256;
  config.graphics.height = 240;
  config.graphics.scale = 3;
  config.graphics.fullscreen = true;

  Game game(config);
  game.loop(new TitleScreen());

  return 0;
}
