/* Film class that handles image output. */

#include "Film.h"
#include "Dist/FreeImage.h"
#include <iostream>

using namespace std;

Film::Film(int width, int height, int BitsPerPixel) {
	FreeImage_Initialise();
	bitmap = FreeImage_Allocate(width, height, BitsPerPixel);
}

void Film::commit(int x, int y, float someColor) {
	// needs to be changed to actually handle colors
	RGBQUAD color;
	color.rgbRed = 0;
	color.rgbGreen = (double) x / 400 * 255.0;
	color.rgbBlue = (double) y / 400 * 255.0;
	FreeImage_SetPixelColor(bitmap, x, y, &color);

	return;
}

void Film::writeImage() {
	if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)) 
		cout << "Image successfully saved!" << endl;

	FreeImage_DeInitialise();

	return;
}