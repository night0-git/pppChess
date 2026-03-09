#ifndef SETTINGS_STATE_HPP
#define SETTINGS_STATE_HPP

#include "State.hpp"
#include "../ui/components/Container.hpp"
#include "../ui/core/SoundPlayer.hpp"
#include <string>

class SettingsState : public State {
public:
    explicit SettingsState(Context& context);

public:
    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;

    void pause() override;
    void resume() override;

private:
    sf::Vector2f _unitSize = sf::Vector2f(800, 100);
    float _verticalSpacing = 20;
    float _horizontalSpacing = 50;
    sf::Font _font = sf::Font("./assets/fonts/Open_Sans/static/OpenSans-Regular.ttf");
    sf::Text _title = sf::Text(_font, "SETTINGS", 50);
    // Currently too lazy to make a dedicated Component class for text
    sf::Text _volumeNum = sf::Text(_font, std::to_string(_context.soundPlayer->masterVolume()), 40);
    ui::VerticalContainer _settingsMenu = ui::VerticalContainer(_unitSize, _verticalSpacing);
};

#endif