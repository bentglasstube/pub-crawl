#pragma once

#include "audio.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

#include "map.h"
#include "player.h"

class MapScreen : public Screen {
  public:

    MapScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override;

  private:

    Map map_;
    Player player_;
    Text text_;
};
