#include "Component.hpp"
#include <cstdint>
#include <algorithm>
using ui::Component, ui::State;

State Component::getState() const {
    return _state;
}

void Component::handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) {}

void Component::update(sf::Time dt) {}

sf::Color Component::lightened(sf::Color color, float amount) {
    float factor = 1.f + amount;
    return sf::Color(
        static_cast<std::uint8_t>(std::min(255.0f, color.r * factor)),
        static_cast<std::uint8_t>(std::min(255.0f, color.g * factor)),
        static_cast<std::uint8_t>(std::min(255.0f, color.b * factor)),
        color.a // Keep transparency
    );
}

sf::Color Component::darkened(sf::Color color, float amount) {
    float factor = 1.f - amount;
    return sf::Color(
        static_cast<std::uint8_t>(color.r * factor),
        static_cast<std::uint8_t>(color.g * factor),
        static_cast<std::uint8_t>(color.b * factor),
        color.a // Keep transparency
    );
}