#include "Engine.h"

Engine::Engine(Coordinates droneCoordinates):
    m_droneCoordinates(droneCoordinates)
{}

Engine::~Engine()
{}

void Engine::update(Events& events)
{
    handleEvents(events);
}

void Engine::handleEvents(Events& events)
{
    
}

Coordinates& Engine::getDroneCoordinates()
{
    return m_droneCoordinates;
}
