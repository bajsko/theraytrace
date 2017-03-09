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
	uint32_t width;
	uint32_t height;
	float fov;
};
bool bol = false;
void computeRay(Ray& ray, const uint32_t x, const uint32_t y, const ImageOptions& options, const vec3f& camOrig, mat44f& camToWorld)
{
	float aspect = (float)options.width / (float)options.height;
	float tanfov = tan(options.fov * 0.5f);


	float xx = (2 * (x + 0.5) / (float)options.width - 1) * aspect * tanfov;
	float yy = (1 - 2 * (y + 0.5) / (float)options.height) * tanfov;

	vec3f camWorldPos;
	vec3f rayWorldPos;

	camToWorld.multVec(camOrig, camWorldPos);
	camToWorld.multVec(vec3f(xx, yy, -1), rayWorldPos);

	ray.pos = camWorldPos;
	ray.dir = (rayWorldPos - camWorldPos).normalize();
}

vec3f castRay(const Ray& ray, const std::vector<Object*>& objects)
{

	vec3f hitColor;

	const Object* object = NULL;
	float t = 0;
	float minDist = INFINITY;
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->intersects(ray, t))
		{
			if (t < minDist)
			{
				object = objects[i];
				minDist = t;
			}
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

	mat44f camToWorld = Mat44Util::look_at(vec3f(0, 0, 0), vec3f(0, 0, -1));

	for (uint32_t y = 0; y < options.height; y++)
	{
		for (uint32_t x = 0; x < options.width; x++)
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
	objects.push_back(new Sphere(vec3f(0, 0, -100), 30, vec3f(1,0,1)));
	objects.push_back(new Sphere(vec3f(10, 0, -50), 20, vec3f(1, 1, 1)));

	ImageOptions options;
	options.width = 640;
	options.height = 480;
	options.fov = 90 * DEG_TO_RAD;

	int i;

	render(options, objects);
}
