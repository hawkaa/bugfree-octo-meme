#include "imageobject.h"
#include <glm\glm.hpp>

ImageObject::ImageObject(ObjectType type, float x, float y, float radius, glm::vec3 color)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->radius = radius;
	this->color = color;
	printf("Laget %f %f %f\n", x, y, radius);
}


ImageObject::~ImageObject(void)
{

}

ObjectType ImageObject::getObjectTypeFromString(char* s) {
	if(strcmp(s, "c") == 0) {
		return ObjectType::CIRCLE;
	} else if (strcmp(s, "p") == 0) {
		return ObjectType::POLYGON;
	} else if(strcmp(s, "s") == 0) {
		return ObjectType::SQUARE;
	} else {
		return ObjectType::TRIANGLE;
	}
};