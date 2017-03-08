//
//  main.cpp
//  theraytracer
//
//  Created by Klas Henriksson on 2017-02-13.
//  Copyright © 2017 bajsko. All rights reserved.
//

#include <iostream>
#include <vector>

#include "vec3.h"
#include "matrix4x4.h"
#include "math_macros.h"
#include "ray.h"
#include "image.h"
#include "geometry.h"

struct ImageOptions
{
	int width;
	int height;
	float fov;
};

void computeRay(Ray& ray, const int x, const int y, const ImageOptions& options, const vec3f& camOrig, mat44f& camToWorld)
{
	float aspect = (float)options.width / (float)options.height;
	float tanfov = tan(options.fov / 2);

	vec3f pixelNDC((x + 0.5f) / (float)options.width, (y + 0.5f) / (float)options.height, 0);
	vec3f pixelScreen(2 * pixelNDC.x - 1, 1 - 2 * pixelNDC.y, 0);
	vec3f pixelCam(pixelScreen.x * aspect * tanfov, pixelScreen.y * tanfov, -1);
	vec3f worldPos;
	camToWorld.multVec(pixelCam, worldPos);

	ray.pos = camOrig;
	ray.dir = worldPos - camOrig;
	ray.dir.normalize();
}

vec3f castRay(const Ray& ray, const std::vector<Object*>& objects)
{

	vec3f hitColor;

	const Object* object = NULL;
	float t = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->intersects(ray, t))
		{
			object = objects[i];
		}
	}

	if (object != NULL)
	{
		hitColor = object->color;
	}

	return hitColor;
}

void render(const ImageOptions& options, const std::vector<Object*>& objects)
{
	vec3f* frameBuffer = new vec3f[options.width * options.height];
	if (!frameBuffer)
		return;

	vec3f* pix = frameBuffer;

	mat44f worldToCam;
	mat44f camToWorld = worldToCam.inverse();

	for (int x = 0; x < options.width; x++)
	{
		for (int y = 0; y < options.height; y++)
		{
			Ray primRay;
			computeRay(primRay, x, y, options, vec3f(0), camToWorld);
			*(pix++) = castRay(primRay, objects);
		}
	}

	Image img(options.width, options.height);
	
	for (int i = 0; i < options.width * options.height; i++)
	{
		vec3f color = frameBuffer[i];
		img.pixels[i].r = color.x;
		img.pixels[i].g = color.y;
		img.pixels[i].b = color.z;
	}

	writePPM("output_raytrace.ppm", img);
	delete[] frameBuffer;
}

int main(int argc, const char * argv[]) {
  
	std::vector<Object*> objects;
	objects.push_back(new Sphere(vec3f(0, 0, -10), 30, vec3f(1,0,1)));

	ImageOptions options;
	options.width = 512;
	options.height = 512;
	options.fov = 90 * DEG_TO_RAD;

	render(options, objects);
}
