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

struct ConfigFont
{
    std::string folderPath;
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

    ConfigFont font;

    ConfigParams(ConfigWindow window, ConfigImage image, ConfigFont font, ConfigMap map) : 
        window(window), image(image), map(map), font(font)
    {}
};

#endif // __CONFIGPARAMS_H__