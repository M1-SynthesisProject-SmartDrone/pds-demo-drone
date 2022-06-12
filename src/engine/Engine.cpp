#include "Engine.h"

#include <channels/channels.h>

Engine::Engine(Coordinates droneCoordinates):
    m_droneCoordinates(droneCoordinates)
{}

Engine::~Engine()
{}

void Engine::update(Events& events)
{
    handleEvents(events);

    m_droneCoordinates.x = pdsChannels::localPositionNed.floats[0];
    m_droneCoordinates.y = pdsChannels::localPositionNed.floats[1];
}

void Engine::handleEvents(Events& events)
{
    
}

Coordinates& Engine::getDroneCoordinates()
{
    return m_droneCoordinates;
}
