#pragma once
#include <GL/glew.h>
#include "renderer.h"

class Renderer;

class Loader
{
public:
	Loader(void);
	~Loader(void);

	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	GLuint loadTexture(const char* texture_file_path);
	void loadObjectsFromFile(const char* file_path);

	void setRenderer(Renderer* renderer);

private:
	Renderer* renderer;
};

