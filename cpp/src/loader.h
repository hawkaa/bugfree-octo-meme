#pragma once
#include <GL/glew.h>
#include "renderer.h"

class Renderer;

enum Filter
{
	NONE,
	BILINEAR,
	TRILINEAR
};


class Loader
{
public:
	Loader(void);
	~Loader(void);

	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	void loadObjectsFromFile(const char* file_path);

	void setFilter(Filter filter);
	void applyFilter();
	GLuint loadBMP(const char* imagepath);

	void setRenderer(Renderer* renderer);

private:
	Renderer* renderer;
	Filter filter;
};

