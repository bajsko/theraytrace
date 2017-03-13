//
//	ray.h
//  theraytracer
//
//	Represents a ray with a direction and position
//
//  Created by Klas Henrikkson   
//  Copyright c 2017 bajsko. All rights reserved
//

#pragma once

#include "vec3.h"

enum RayType
{
    kRayTypePrimary,
    kRayTypeShadow,
};

class Ray
{

public:
	Ray() : dir(0,0,-1), pos(0), type(kRayTypePrimary) {}
	Ray(const vec3f& o, const vec3f& d) : pos(o), dir(d), type(kRayTypePrimary) {}
	Ray(float x, float y, float z, float dx, float dy, float dz) : pos(x, y, z), dir(dx, dy, dz), type(kRayTypePrimary) {}

	vec3f dir;
	vec3f pos;
    float tMin = 0;
    float tMax = INFINITY;
    RayType type;
};
