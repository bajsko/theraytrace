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
    
    virtual void getShadingInfo(const vec3f& hitPoint, vec3f& lightDir, vec3f& lightIntensity, float& dist) const = 0;
    
    mat44f lightToWorld;
    vec3f color;
    float intensity;
};

class DistantLight : public Light
{
public:
    DistantLight(const mat44f& l2w, const vec3f& c, const float& i) :
    Light(l2w, c, i)
    {
        lightToWorld.multDirVec(vec3f(0,0,-1), dir);
        dir.normalize();
        //dir = vec3f(0,-5,-5).normalize();
    }
    
    void getShadingInfo(const vec3f& hitPoint, vec3f& lightDir, vec3f& lightIntensity, float& dist) const
    {
        lightDir = dir;
        lightIntensity = color * intensity * (1/M_PI);
        dist = INFINITY;
    }
    
    vec3f dir;
};

class PointLight : public Light
{
public:
    PointLight(const mat44f& l2w, const vec3f& c, const float& i) : Light(l2w, c, i)
    {
        l2w.multVec(vec3f(0,0,0), pos);
    }
    
    void getShadingInfo(const vec3f& P, vec3f& lightDir,
                                  vec3f& lightIntensity, float& dist) const
    {
        lightDir = P - pos;
        float r2 = lightDir.length();
        dist = sqrt(r2);
        lightDir.normalize();
        lightIntensity = (color * intensity) * (1/(4*M_PI*r2));
    }
    
    vec3f pos;
};

#endif /* light_h */
