/* Film class that handles image output. */

#ifndef __FILM_H__
#define __FILM_H__
#include "Film.h"
#endif

#include "Dist/FreeImage.h"
#include <iostream>
#include "glm/glm.hpp"

using namespace std;

Film::Film(int width, int height, int BitsPerPixel) {
	FreeImage_Initialise();
	bitmap = FreeImage_Allocate(width, height, BitsPerPixel);
}

void Film::commit(int x, int y, glm::vec3 color) {
	RGBQUAD free_image_color;
	free_image_color.rgbRed = color[0] * 255.0f;
	free_image_color.rgbGreen = color[1] * 255.0f;
	free_image_color.rgbBlue = color[2] * 255.0f;
	FreeImage_SetPixelColor(bitmap, x, y, &free_image_color);

	return;
}

void Film::writeImage() {
	if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)) 
		cout << "Image successfully saved!" << endl;

	FreeImage_DeInitialise();

	return;
}