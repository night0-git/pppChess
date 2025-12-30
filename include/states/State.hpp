#ifndef _STATE_H_
#define _STATE_H_

#include <SFML/Graphics.hpp>

struct Context;

class State {
public:
    explicit State(Context& context) : _context(context) {};
    virtual ~State() = default;

public:
    virtual void init() = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

protected:
    Context& _context;
};

#endif