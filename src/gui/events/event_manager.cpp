#include "event_manager.h"

void updateEvents(sf::RenderWindow& window, Events& events)
{
    events.reset();
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close the window (CTRL-C or clicking the button)
        if (event.type == sf::Event::Closed)
        {
            events.isWindowClosed = true;
        }
        // Keyboard one button press (KeyPressed is checked each frame)
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            default:
                break;
            }
        }
    }
}
