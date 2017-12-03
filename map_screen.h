#pragma once

#include "audio.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

#include "map.h"
#include "game_state.h"

class MapScreen : public Screen {
  public:

    MapScreen(GameState state);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override;

  private:

    static constexpr int kStartTime = 20;

    GameState state_;
    Text text_;
};
