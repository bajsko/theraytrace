//
//  disk.cpp
//  theraytracer
//
//  Created by Klas Henriksson on 2017-03-13.
//  Copyright © 2017 bajsko. All rights reserved.
//

#include "geometry.h"

bool Disk::intersects(const Ray &ray, float &t) const
{
    vec3f normalized_normal = Vec3Util::normalize(normal);
    float denom = ray.dir.dot(normalized_normal);
    if(fabs(denom) > 1e-6)
    {
        vec3f p0l0 = center - ray.pos;
        t = p0l0.dot(normalized_normal) / denom;
        if(t >= 0)
        {
            float rad2 = radius * radius;
            vec3f hit = ray.pos + ray.dir * t;
            if((hit - center).length() > rad2)
                return false;
            
            return true;
        }
    }
    
    return false;
}

void Disk::getSurfaceData(const vec3f &hit, vec3f &normal, vec3f &texCoord) const
{
    normal = Vec3Util::normalize(this->normal);
}
