#ifndef __EVENTS_H__
#define __EVENTS_H__

/**
 * This holds all important keys that are pressed / released
 * on a particular frame
 */
struct Events
{
    // Special case handled by main function
    bool isWindowClosed = false;

    void reset()
    {
    }
};


#endif // __EVENTS_H__