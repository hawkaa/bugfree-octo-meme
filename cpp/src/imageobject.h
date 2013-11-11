#pragma once
#include <glm\glm.hpp>

enum ObjectType
{
	CIRCLE,
	SQUARE,
	TRIANGLE,
	POLYGON
};

class ImageObject
{
public:
	ImageObject(ObjectType type, float x, float y, float radius, glm::vec3 color);
	~ImageObject(void);

	static ObjectType getObjectTypeFromString(char* s);

};

