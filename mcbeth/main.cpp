#include <stdio.h>
#include "image.h"

int main(int argc, char** argv) {

	Image* img;
	img = readPPM("test.ppm");
	if (!img)
		return -1;
	printf("Loaded image, width: %d, height: %d\n", img->getWidth(), img->getHeight());

	writePPM("test_output.ppm", *img);
	int i = 0;
	std::cin >> i;

	delete img;

	return 0;
}