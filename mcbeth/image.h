//
//  image.h
//  theraytracer
//
//	Contains functions for dealing with images
//
//  Created by Klas Henrikkson   
//  Copyright c 2017 bajsko. All rights reserved
//

#ifndef image_h
#define image_h

#include <stdio.h>
#include "vec3.h"

struct RGB
{
	unsigned char r, g, b;
};

// Represents a basic image holding R, G and B data.
class Image
{
private:
	int w, h;
public:

	Image() : w(0), h(0), pixels(nullptr) {}
	Image(int width, int height) : w(width), h(height), pixels(nullptr) { pixels = new RGB[w * h]; memset(pixels, 0, sizeof(RGB) * w * h); }
	~Image() { if (pixels) delete[] pixels; }

	int getWidth() const { return w;  }
	int getHeight() const { return h; }

	const RGB operator[] (const unsigned int &i) const { return pixels[i]; }
	RGB operator[] (const unsigned int &i) { return pixels[i]; }

	RGB* pixels;

	static const RGB kBlack, kWhite, kRed, kGreen, kBlue;
};

const RGB Image::kBlack = { 0, 0, 0 };
const RGB Image::kWhite = { 0, 0, 0 };
const RGB Image::kRed = { 0, 0, 0 };
const RGB Image::kGreen = { 0, 0, 0 };
const RGB Image::kBlue = { 0, 0, 0 };

//Tries to read a PPM image file.
//string = path to file
//returns a Image pointer on success and a NULL reference if failure
//TODO: add error handling (e.g getImageIOError())
Image* readPPM(const char* string)
{
	if (!string)
		return nullptr;

	FILE* file = fopen(string, "rb");
	if (!file)
		return nullptr;
	/*char magic[3] = { 0 };
	fread(magic, sizeof(char), sizeof(magic), file);

	if (ferror)
		return nullptr;

	if ((magic[0] != 'P' || magic[1] != '6'))
		return nullptr;*/
	unsigned long size = 0;
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	char* buffer = (char*)malloc(sizeof(char) * size);
	if (!buffer)
		return nullptr;

	fread(buffer, sizeof(char), size, file);

	if (buffer[0] != 'P' || buffer[1] != '6') {
		fclose(file);
		free((void*)buffer);
		return nullptr;
	}

	char* size_offset = buffer + 3;
	char* p = size_offset;
	while (*p != '\n' && *p != ' ' && *p != '\t' && *p != '\0') {	
		p++;
	}

	int width = atoi(size_offset);
	int height = atoi(p);

	p++;
	while (*p != '\n' && *p != ' ' && *p != '\t' && *p != '\0') {
		p++;
	}

	int maxval = atoi(p);
	if (maxval > 255) {
		fclose(file);
		free((void*)buffer);
		return nullptr;
	}

	p++;
	while (*p != '\n' && *p != ' ' && *p != '\t' && *p != '\0') {
		p++;
	}

	Image* img = new Image(width, height);
	if (!img)
	{
		fclose(file);
		free((void*)buffer);
		return nullptr;
	}

	char* pixel_buff = p+1;
	for (int i = 0; i < (width*height*3); i+=3)
	{
		unsigned char r = pixel_buff[i];
		unsigned char g = pixel_buff[i + 1];
		unsigned char b = pixel_buff[i + 2];
		RGB pixel = { r, g, b };
		img->pixels[i/3] = pixel;
	}

	fclose(file);

	return img;
}

//tries to write a PPM file from a Image class
//writes with 255 maxval (1-byte per color component)
//returns 0 if success, otherwise non-zero.
int writePPM(const char* dest, const Image& img) {

	FILE* file = fopen(dest, "wb");
	if (!file)
		return -1;

	int w = img.getWidth();
	int h = img.getHeight();

	char magic_header[3] = { 'P', '6', ' '};
	fwrite(magic_header, sizeof(char), sizeof(magic_header), file);

	fclose(file);

	return 0;
}

#endif
