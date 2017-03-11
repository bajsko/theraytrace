//
//  plane.cpp
//  theraytracer
//
//  Created by Klas Henriksson on 2017-03-11.
//  Copyright © 2017 bajsko. All rights reserved.
//

#include "geometry.h"

bool Plane::intersects(const Ray &ray, float &t) const
{
    vec3f normalized_normal = Vec3Util::normalize(normal);
    float denom = ray.dir.dot(normalized_normal);
    if(fabs(denom) > 1e-6)
    {
        vec3f p0l0 = center - ray.pos;
        t = p0l0.dot(normalized_normal) / denom;
        return (t >= 0);
    }
    
    return false;
}

void Plane::getSurfaceData(const vec3f &hit, vec3f &normal, vec3f &texCoord) const
{
    vec3f planePos = hit - center;
    float bounds = 1000;
    
    normal = Vec3Util::normalize(this->normal);
    texCoord.x = fmodf(planePos.x, bounds)/bounds;
    texCoord.y = fmodf(planePos.y, bounds)/bounds;
}
