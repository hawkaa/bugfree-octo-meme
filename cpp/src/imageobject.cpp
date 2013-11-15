#include "imageobject.h"
#include <glm\glm.hpp>
#include <stdio.h>
#include <string>

ImageObject::ImageObject(ObjectType type, float x, float y, float radius, glm::vec4 color, int colorIndex)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->radius = radius;
	this->color = color;
	this->colorIndex = colorIndex;
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

float ImageObject::getX() { return this->x; };
float ImageObject::getY() { return this->y; };
float ImageObject::getRadius() { return this->radius; };
int ImageObject::getColorIndex() { return this->colorIndex; };
glm::vec4 ImageObject::getColor() { return this->color; };
ObjectType ImageObject::getType() { return this->type; };