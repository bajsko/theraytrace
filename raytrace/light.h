//
//  light.hpp
//  theraytracer
//
//  Created by Klas Henriksson on 2017-03-11.
//  Copyright © 2017 bajsko. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include "vec3.h"
#include "matrix4x4.h"

class Light
{
public:
    Light(const mat44f& l2w) : lightToWorld(l2w) {}
    Light(const mat44f& l2w, const vec3f& c, const float& i) : lightToWorld(l2w),
    color(c.x, c.y, c.z), intensity(i) {}
    virtual ~Light() {}
    
    mat44f lightToWorld;
    vec3f color;
    float intensity;
};

class DistantLight : public Light
{
    DistantLight(const mat44f& l2w, const vec3f& c, const float& i) :
    Light(l2w, c, i)
    {
        l2w.multDirVec(vec3f(0,0,-1), dir);
    }
    
    vec3f dir;
};

class PointLight : public Light
{
    
};

#endif /* light_h */
