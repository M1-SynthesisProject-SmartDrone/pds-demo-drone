#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__


#include <SFML/Graphics.hpp>

#include "Events.h"

/**
 * Check all events happening during the current frame.
 */
void updateEvents(sf::RenderWindow& window, Events& events);

#endif // __EVENT_MANAGER_H__