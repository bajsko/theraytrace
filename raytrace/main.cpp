//
//  main.cpp
//  theraytracer
//
//  Created by Klas Henriksson on 2017-02-13.
//  Copyright © 2017 bajsko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <time.h>

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

inline vec3f mix(const vec3f& a, const vec3f& b, const float& t)
{
	return vec3f(a.x*(1 - t) + b.x*t, a.y*(1 - t) + b.y*t, a.z*(1 - t) + b.z*t);
}

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

vec3f castRay(const Ray& ray, const std::vector<Object*>& objects, vec3f lightPos)
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
		vec3f pHit = ray.pos + (ray.dir * t);
		Ray shadowRay;
		shadowRay.dir = (lightPos - pHit).normalize();
		shadowRay.pos = pHit;
		
		bool inShadow = false;

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->intersects(shadowRay, t))
			{
				inShadow = true;
				break;
			}
		}

		Sphere* sphere = (Sphere*)object;
		vec3f norm = (pHit - sphere->center).normalize();

		float phi = (1 + atan2(norm.z, norm.x)) / M_PI;
		float theta = acos(norm.y) / M_PI;
		float scale = 8;
		float pattern = (fmodf(theta * scale, 1) > 0.5) ^ (fmodf(phi * scale, 1) > 0.5);
		hitColor = mix(object->color, object->color * 0.8f, pattern) * norm.dot(shadowRay.dir);
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
			*(pix++) = castRay(primRay, objects, vec3f(100,40,-50));
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

inline float rand01()
{
	return (float)rand() / (float)RAND_MAX;
}

int main(int argc, const char * argv[]) {
  
	srand(time(NULL));

	std::vector<Object*> objects;
	float r = rand01() * 10;
	int spheres = (int)(r);
	for (int i = 0; i < spheres; i++)
	{
		vec3f pos = vec3f(rand01() * 50, rand01() * 100, -(rand01() * 120));
		float radius = rand01() * 35 + 1;
		vec3f color = vec3f(rand01(), rand01(), rand01());
		objects.push_back(new Sphere(pos, radius, color));
	}

	ImageOptions options;
	options.width = 640;
	options.height = 480;
	options.fov = 90 * DEG_TO_RAD;

	int i;

	render(options, objects);
}
