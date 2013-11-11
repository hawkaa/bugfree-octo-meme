#include <GL/glew.h>
#include "octobrain.h"

int main(int argc, char** argv)
{
	Octobrain* brain = new Octobrain();
	brain->init();
	brain->run();
}


