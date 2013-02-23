/* Film header */
#include "Dist/FreeImage.h"

class Film
{
public:
	FIBITMAP * bitmap;
	int BitsPerPixel;

	Film(int,int,int);
	void commit(int,int,float);
	void writeImage();
};