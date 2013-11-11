#include <GL/glew.h>
#include <GL/glfw3.h>
#include "input.h"
#include "renderer.h"
#include "camera.h"

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
	static const char* objectsFileName;
private:
	Input* input;
	Loader* loader;
	Renderer* renderer;
	Camera* camera;


};