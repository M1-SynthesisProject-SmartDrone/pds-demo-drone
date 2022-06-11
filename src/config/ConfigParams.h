#ifndef __CONFIGPARAMS_H__
#define __CONFIGPARAMS_H__

#include <string>

struct ConfigWindow
{
    std::string name;
    int width;
    int height;
    bool enableVsync;
    int framerate;
};

struct ConfigImage
{
    std::string folderPath;
    bool enableSmooth;
};

struct ConfigMap
{
    float width;
    float height;
};

/**
 * The struct containing all config params
 */
struct ConfigParams
{
    ConfigImage image;

    ConfigWindow window;

    ConfigMap map;

    ConfigParams(ConfigWindow window, ConfigImage image, ConfigMap map) : 
        window(window), image(image), map(map)
    {}
};

#endif // __CONFIGPARAMS_H__