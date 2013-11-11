#include <GL/glew.h>
#include <GL/glfw3.h>
#include "input.h"
#include "renderer.h"
#include "camera.h"

int lol[5][3];

class Octobrain
{
public:
	GLFWwindow* window;

	Octobrain(void);
	~Octobrain(void);

	void init();
	void run();
	void cleanup();

	static const int screenWidth = 800, screenHeight = 600;

private:
	Input* input;
	Renderer* renderer;
	Camera* camera;
};