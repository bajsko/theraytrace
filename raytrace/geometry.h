//
//	ray.h
//  theraytracer
//
//	Contains basic classes for dealing with the most common geometric shapes
//
//  Created by Klas Henrikkson   
//  Copyright c 2017 bajsko. All rights reserved
//

#pragma once

#include "ray.h"

class Object
{
public:

	Object() { color = vec3f(0); }
	Object(const vec3f& c) : color(c.x, c.y, c.z) {}
	Object(float r, float g, float b) : color(r, g, b) {}

	virtual ~Object() {}
	virtual bool intersects(const Ray& ray, float& t) const = 0;

	vec3f color;
};

class Sphere : public Object
{
public:
	Sphere(const vec3f& cent, const float r) : radius(r), center(cent.x, cent.y, cent.z) {}
	Sphere(const vec3f& cent, const float r, const vec3f& clr) : center(cent.x, cent.y, cent.z), radius(r), Object(clr) {}
	~Sphere() {}

	bool intersects(const Ray& ray, float& t) const;
	float radius2() const;

	float radius;
	vec3f center;
};