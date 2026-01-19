#include "../../../include/ui/components/RoundedRect.hpp"
#include <cmath>
#include <algorithm>

RoundedRect::RoundedRect(sf::Vector2f size, float radius, unsigned int cornerPoints)
: _size(size), _radius(radius), _cornerPoints(cornerPoints) {}

sf::Vector2f RoundedRect::getSize() const {
    return _size;
}

void RoundedRect::setSize(sf::Vector2f size) {
    _size = size;
    float maxRadius = std::min(_size.x, _size.y) / 2.f;
    if (_radius > maxRadius) {
        _radius = maxRadius;
    }
    update();
}

void RoundedRect::setCornerRadius(float radius, unsigned int cornerPoints) {
    float maxRadius = std::min(_size.x, _size.y) / 2.f;
    _radius = std::clamp(radius, 0.f, maxRadius);
    _cornerPoints = cornerPoints;
    update();
}

std::size_t RoundedRect::getPointCount() const {
    return _cornerPoints * 4;
}

sf::Vector2f RoundedRect::getPoint(std::size_t index) const {
    if (index >= getPointCount()) {
        return {0, 0};
    }

    float deltaAngle = 90.f / (_cornerPoints - 1);
    int cornerIdx = index / _cornerPoints;
    int offsetIdx = index % _cornerPoints;

    sf::Vector2f cornerCenter;
    switch (cornerIdx) {
        case 0: cornerCenter = {_size.x - _radius, _size.y - _radius}; break;
        case 1: cornerCenter = {_radius, _size.y - _radius}; break;
        case 2: cornerCenter = {_radius, _radius}; break;
        case 3: cornerCenter = {_size.x - _radius, _radius}; break;
    }

    float angle = (cornerIdx * 90.f) + (offsetIdx * deltaAngle);
    float x = std::cos(angle * 3.14159265f / 180.f) * _radius;
    float y = std::sin(angle * 3.14159265f / 180.f) * _radius;

    return cornerCenter + sf::Vector2f(x, y);
}