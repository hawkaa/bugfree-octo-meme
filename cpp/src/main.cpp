#include <GL/glew.h>
#include "octobrain.h"
#include "text.h"

int main(int argc, char** argv)
{
	Octobrain* brain = new Octobrain();
	brain->init();
	brain->run();
}


