#pragma once

#include "audio.h"
#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

#include "game_state.h"

class BarScreen : public Screen {
  public:

    BarScreen(GameState state, const Building& pub);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override;

  private:

    GameState state_;
    const Building& pub_;
    Backdrop backdrop_;
    Text text_;
};
