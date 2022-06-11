#ifndef __MAPINFOS_H__
#define __MAPINFOS_H__

struct MapInfos
{
    float minX;
    float maxX;
    float minY;
    float maxY;
    float width;
    float height;

    MapInfos(float width, float height) : width(width), height(height)
    {
        float midWidth = width / 2.f;
        float midHeight = height / 2.f;
        this->minX = -midWidth;
        this->maxX = midWidth;
        this->minY = -midHeight;
        this->maxY = midHeight;
    }
};

#endif // __MAPINFOS_H__