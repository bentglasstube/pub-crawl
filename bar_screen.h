#pragma once

#include "audio.h"
#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

#include "game_state.h"
#include "message_box.h"

class BarScreen : public Screen {
  public:

    BarScreen(GameState state, const Building& pub);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override;

  private:

    enum class Phase { Greeting, BeerMenu, BeerInfo, Ordering, Drinking, Paying, Drunk, Dishes };

    GameState state_;
    const Building& pub_;
    Backdrop backdrop_;
    Text text_;
    std::unique_ptr<MessageBox> msg_;
    Phase phase_;
    const Beer* beer_;
    int tab_;

    std::string format_beer_info(const Beer* beer);
    void handle_menu_choice();
    void set_phase(Phase phase_);
};
