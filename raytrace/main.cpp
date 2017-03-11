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
#include <algorithm>
#include <utility>
#include <limits>

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

bool trace(const Ray& ray, const std::vector<Object*>& objects, Object*& hitObject, float& minDist)
{
    std::vector<Object*>::const_iterator it = objects.begin();
    float t = INFINITY;
    minDist = INFINITY;
    hitObject = NULL;
    for(; it != objects.end(); it++)
    {
        if((*it)->intersects(ray, t) && t < minDist)
        {
            hitObject = (*it);
            minDist = t;
        }
    }
    
    return (hitObject != NULL);
}

vec3f castRay(const Ray& ray, const std::vector<Object*>& objects, vec3f lightPos)
{

	vec3f hitColor;
    Object* object = NULL;
    
    float minDist;
    
	if (trace(ray, objects, object, minDist))
	{
		vec3f pHit = ray.pos + (ray.dir * minDist);
        vec3f norm;
        vec3f texCoord;
        
        object->getSurfaceData(pHit, norm, texCoord);
        
        Ray shadowRay(pHit + norm * 1e-5, vec3f(0));
        shadowRay.dir = (lightPos - shadowRay.pos).normalize();
        Object* shadowObject = NULL;
        bool inShadow = false;
        inShadow = !trace(shadowRay, objects, shadowObject, minDist);
        
		float scale = 16;
		float pattern = (fmodf(texCoord.y * scale, 1) > 0.5) ^ (fmodf(texCoord.x * scale, 1) > 0.5);
        vec3f invRayDir = vec3f(-ray.dir.x, -ray.dir.y, -ray.dir.z);
        
        hitColor = mix(object->color, object->color * 0.8f, pattern) * norm.dot((lightPos - pHit).normalize()) * inShadow;
        clamp<float>(hitColor.x, 0.0f, 1);
        clamp<float>(hitColor.y, 0.0f, 1);
        clamp<float>(hitColor.z, 0.0f, 1);
	}

	return hitColor;
}

void render(const ImageOptions& options, const std::vector<Object*>& objects)
{
	vec3f* frameBuffer = new vec3f[options.width * options.height];
	if (!frameBuffer)
		return;

	vec3f* pix = frameBuffer;

	mat44f camToWorld = Mat44Util::look_at(vec3f(0, 5.5f, 40), vec3f(0, 0, -100));

	for (uint32_t y = 0; y < options.height; y++)
	{
		for (uint32_t x = 0; x < options.width; x++)
		{
			Ray primRay;
			computeRay(primRay, x, y, options, vec3f(0), camToWorld);
			*(pix++) = castRay(primRay, objects, vec3f(0,40,-15));
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
    
    objects.push_back(new Plane(vec3f(0,0,-100), vec3f(0,1,0), vec3f(1.0/2.0f,1.0/2.0f,1.0/2.0f)));
    objects.push_back(new Sphere(vec3f(0,10,-15), 5, vec3f(1,1,1)));
    objects.push_back(new Sphere(vec3f(-10,10,-15), 5, vec3f(1,1,1)));
    objects.push_back(new Sphere(vec3f(10,10,-15), 5, vec3f(1,1,1)));
    
    std::cout << "num spheres: " << objects.size() << std::endl;

	ImageOptions options;
	options.width = 640;
	options.height = 480;
	options.fov = 120 * DEG_TO_RAD;

	int i;

	render(options, objects);
}
