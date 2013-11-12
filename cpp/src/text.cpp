#include "text.h"
#include <vector>
#include "stdio.h"

Text::Text(void)
{
	this->hasInitialized = false;
}


Text::~Text(void)
{
}

void Text::addNumberTexture(GLuint texture, int number)
{
	if((number < 10 && number > 0) || this->textsAdded[number])
	{
		this->textsAdded[number] = true;
		this->textures[number] = texture;	
	}
	this->checkIfInitialized();
}

void Text::checkIfInitialized()
{
	for(int i = 0; i < 10; ++i)
	{
		if(!this->textsAdded[i])
		{
			return;
		}
	}
	this->hasInitialized = true;
}


void Text::addNumber(int number, glm::vec3 position)
{
	//if(!this->hasInitialized)
	//	return;

	std::vector<int> numbers;
	while(number > 1)
	{
		int residue = number%10;
		number /= 10;
		printf("Number %i, residue %i\n", number, residue);
		numbers.push_back(residue);
	}
}
