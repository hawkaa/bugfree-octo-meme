#include <GL/glew.h>
#include <GL/glfw3.h>
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "text.h"

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

	static const char* textureFileNames[10];
private:
	Input* input;
	Loader* loader;
	Renderer* renderer;
	Camera* camera;
	Text* text;
};