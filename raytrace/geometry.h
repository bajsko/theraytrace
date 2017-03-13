//
//	geometry.h
//  theraytracer
//
//	Contains basic classes for dealing with the most common geometric shapes
//
//  Created by Klas Henrikkson   
//  Copyright c 2017 bajsko. All rights reserved
//

#pragma once

#include "ray.h"

enum ObjectType
{
    kDiffuse,
    kReflection,
};

class Object
{
public:

	Object() { albedo = vec3f(0); }
	Object(const vec3f& c) : albedo(c.x, c.y, c.z) {}
	Object(float r, float g, float b) : albedo(r, g, b) {}

	virtual ~Object() {}
	virtual bool intersects(const Ray& ray, float& t) const = 0;
    virtual void getSurfaceData(const vec3f& hit, vec3f& normal, vec3f& texCoord) const = 0;

	vec3f albedo;
    ObjectType type = kDiffuse;
};

class Sphere : public Object
{
public:
	Sphere(const vec3f& cent, const float r) : radius(r), center(cent.x, cent.y, cent.z) {}
	Sphere(const vec3f& cent, const float r, const vec3f& alb) : center(cent.x, cent.y, cent.z), radius(r), Object(alb) {}
	~Sphere() {}

	bool intersects(const Ray& ray, float& t) const;
    void getSurfaceData(const vec3f& hit, vec3f& normal, vec3f& texCoord) const;
	float radius2() const;

	float radius;
	vec3f center;
};

class Plane : public Object
{
public:
    Plane(const vec3f& cntr, const vec3f& norm) : center(cntr.x, cntr.y, cntr.z), normal(norm.x, norm.y, norm.z) {}
    Plane(const vec3f& cntr, const vec3f& norm, const vec3f& alb) : center(cntr.x, cntr.y, cntr.z),
    normal(norm.x, norm.y, norm.z), Object(alb) {}
    
    bool intersects(const Ray& ray, float& t) const;
    void getSurfaceData(const vec3f& hit, vec3f& normal, vec3f& texCoord) const;
    
    vec3f center;
    vec3f normal;
};

class Disk : public Object
{
public:
    Disk(const vec3f& cntr, const vec3f& norm, const float rad) :
    center(cntr), normal(norm), radius(rad) {}
    Disk(const vec3f& cntr, const vec3f& norm, const float rad, const vec3f& alb) :
    center(cntr), normal(norm), radius(rad), Object(alb) {}
    
    bool intersects(const Ray& ray, float& t) const;
    void getSurfaceData(const vec3f& hit, vec3f& normal, vec3f& texCoord) const;
    
    vec3f center;
    vec3f normal;
    float radius;
};
