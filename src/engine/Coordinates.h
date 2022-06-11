#ifndef __COORDINATES_H__
#define __COORDINATES_H__

struct Coordinates {
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float rotation = 0.0f; 

		Coordinates();
		Coordinates(float x, float y, float z, float r);
};
#endif // __COORDINATES_H__