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
	
	float getX();
	float getY();
	float getRadius();
	ObjectType getType();
	glm::vec3 getColor();

private:
	float x,y,radius;
	ObjectType type;
	glm::vec3 color;

};

