#define _CRT_SECURE_NO_DEPRECATE
#include "loader.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

#include <glm\glm.hpp>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include "loader.h"
#include "imageobject.h"

using namespace std;

Loader::Loader(void)
{

}

Loader::~Loader(void)
{

}

void Loader::setRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

void Loader::loadObjectsFromFile(const char* file_path) {
	int numColors;
	float tmpColors[4];
	std::vector<glm::vec4> colors;
	glm::vec4 color;

	float x, y, r;
	int c, res;
	char s[128];

	FILE* file = fopen(file_path, "r"); // Read-only

	if(file == NULL) {
		printf("Could not open objects file at: '%s'\n", file_path);
		return;
	}

	// Reading number of lines
	
	fscanf(file, "%i\n", &numColors);

	/*
	 * Create color array
	 */
	for(int i = 0; i < numColors; ++i)
	{
		// Read file
		fscanf(file, "%f %f %f %i\n", &tmpColors[0], &tmpColors[1], &tmpColors[2], &tmpColors[3]);

		// Create color vector
		color.x = tmpColors[1]/255;
		color.y = tmpColors[2]/255;
		color.z = tmpColors[3]/255;
		color.w = 1.0f;

		// Add color vector to colors vector
		colors.push_back(color);
	}

	/*
	 * Loop through end of file to find all the vertices
	 */

	while(true) {
		res = fscanf(file, "%f %f %f %s %i\n", &x, &y, &r, &s, &c);
		if(res == EOF) {
			break;
		}
		ImageObject io = ImageObject(ImageObject::getObjectTypeFromString(s),x,y,r,colors[c-1]);
		this->renderer->addImageObject(io);
	}
}


void Loader::setFilter(Filter newFilter)
{
	filter = newFilter;		
}

void Loader::applyFilter()
{
	switch(filter)
	{
	case NONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	case BILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	case TRILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

GLuint Loader::loadBMP(const char* imagepath)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char* data;

	printf("Loading texture ... %s", imagepath);

	FILE* file = fopen(imagepath, "rb");
	if(!file)
	{
		printf("Could not open image: %s\n", imagepath);
		return 0;
	}

	// Check if header is valid
	if(fread(header, 1, 54, file) != 54)
	{
		printf("%s's header is invalid.", imagepath);
	}

	if(header[0] != 'B' || header[1] != 'M')
	{
		printf("%s is not a BMP image", imagepath);
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Illformated BMP, make an assumption
	if(imageSize == 0)
		imageSize = width*height*3;

	if(dataPos == 0)
		dataPos = 54;
	
	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	applyFilter();

	printf(" - Texture loaded succesfully\n");

	return textureId;
}

/*
HÅKON SE
*/
/*
bool loadObj(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE* file = fopen(path, "r");
	if(file == NULL)
	{
		printf("Object path not found: %s", path);
		return false;
	}

	while(true)
	{
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if(res == EOF)
			break;

		if(strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9)
			{
				printf("Object file too complex for the parser, export with different settings!");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	for(unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertIndex - 1];
		vertices.push_back(vertex);
	}

	for(unsigned int j = 0; j < uvIndices.size(); j++)
	{
		unsigned int uvIndex = uvIndices[j];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
	}

	for(unsigned int k = 0; k < normalIndices.size(); k++)
	{
		unsigned int normalIndex = normalIndices[k];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}
	printf("Object loaded succesfully with %d vertices", vertices.size());
	return true;
}

struct PackedVertex
{
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getVertexIndex(PackedVertex &packed, std::map<PackedVertex, unsigned short> &outIndices, unsigned short &result)
{
	std::map<PackedVertex, unsigned short>::iterator it = outIndices.find(packed);
	
	if(it == outIndices.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	std::map<PackedVertex, unsigned short> tempOutIndices;	
	printf("\nIndexing VBO with %d vertices...", in_vertices.size());
	for(unsigned int i = 0; i < in_vertices.size(); i++)
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};

		unsigned short tempIndex = 0;
		bool exists = getVertexIndex(packed, tempOutIndices, tempIndex);
	
		if(exists)
		{
			out_indices.push_back(tempIndex);
		}
		else
		{
		
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			
			tempIndex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(tempIndex);
			tempOutIndices[packed] = tempIndex;
		}
	}
	printf(" Indexing completed\n"); 
}
*/
	/*
	HÅKON IKKE SE MER!
	*/

GLuint Loader::LoadShaders(const char* vertex_file_path, const char* fragment_file_path){

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		printf("Cant open %s", vertex_file_path);
		return 0;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}