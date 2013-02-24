/* Film header */
#include "Dist/FreeImage.h"
#include "glm/glm.hpp"

class Film
{
public:
	FIBITMAP * bitmap;
	int BitsPerPixel;

	Film(int,int,int);
	void commit(int,int,glm::vec3);
	void writeImage();
};