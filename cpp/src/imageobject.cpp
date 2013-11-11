#include "imageobject.h"
#include <glm\glm.hpp>

ImageObject::ImageObject(ObjectType type, float x, float y, float radius, glm::vec3 color)
{

}


ImageObject::~ImageObject(void)
{

}

ObjectType ImageObject::getObjectTypeFromString(char* s) {
	if(s == "c") {
		return ObjectType::CIRCLE;
	} else if (s == "p") {
		return ObjectType::POLYGON;
	} else if(s == "s") {
		return ObjectType::SQUARE;
	} else {
		return ObjectType::TRIANGLE;
	}
};