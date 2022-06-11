#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "gui/events/Events.h"
#include "Coordinates.h"
#include "config/ConfigParams.h"

class Engine
{
private:
    static constexpr float MOVEMENT_SPEED = 1.0f;
    
    Coordinates m_droneCoordinates;

    // ==== Private methods ====
    void handleEvents(Events& events);
public:
    Engine(Coordinates droneCoordinates = {});
    ~Engine();

    void update(Events& events);

    // ==== GETTERS ====
    Coordinates& getDroneCoordinates();
};

#endif // __ENGINE_H__