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
#include "light.h"

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
        if((*it)->intersects(ray, t) && t < minDist && t < ray.tMax)
        {
            hitObject = (*it);
            minDist = t;
        }
    }
    
    return (hitObject != NULL);
}

vec3f castRay(const Ray& ray, const std::vector<Object*>& objects,
              const std::vector<Light*>& lights)
{
    float bias = 1e-5;
    
    vec3f hitColor;
    Object* object = NULL;
    
    float minDist;
    
    if (trace(ray, objects, object, minDist))
    {
        vec3f pHit = ray.pos + (ray.dir * minDist);
        vec3f norm;
        vec3f texCoord;
        
        object->getSurfaceData(pHit, norm, texCoord);
        
        for(int i = 0; i < lights.size(); i++)
        {
            vec3f lightDir;
            vec3f lightIntensity;
            float lightDist = 0;
            
            lights[i]->getShadingInfo(pHit, lightDir, lightIntensity, lightDist);
            
            Object* shadowObject = NULL;
            Ray shadowRay = Ray(pHit + norm * bias, lightDir * -1);
            shadowRay.type = kRayTypeShadow;
            shadowRay.tMax = lightDist;
            
            bool vis = !trace(shadowRay, objects, shadowObject, minDist);
            
            hitColor += object->albedo * lightIntensity * vis * std::max(0.0f, norm.dot(lightDir * -1));
        }
    }
    
    clamp<float>(hitColor.x, 0.0f, 1);
    clamp<float>(hitColor.y, 0.0f, 1);
    clamp<float>(hitColor.z, 0.0f, 1);
    
    return hitColor;
}

void render(const ImageOptions& options, const std::vector<Object*>& objects,
            const std::vector<Light*>& lights)
{
    vec3f* frameBuffer = new vec3f[options.width * options.height];
    if (!frameBuffer)
        return;
    
    vec3f* pix = frameBuffer;
    
    mat44f camToWorld = Mat44Util::look_at(vec3f(0, 10, -20), vec3f(0, 3, 0));
    
    for (uint32_t y = 0; y < options.height; y++)
    {
        for (uint32_t x = 0; x < options.width; x++)
        {
            Ray primRay;
            computeRay(primRay, x, y, options, vec3f(0), camToWorld);
            *(pix++) = castRay(primRay, objects, lights);
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
    
    srand((unsigned int)(time(NULL)));
    vec3f v(5,12,12);
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    
    objects.push_back(new Disk(vec3f(0,-1.0f,0), vec3f(0,1,0), 30, vec3f(0.18f)));
    objects.push_back(new Sphere(vec3f(-5,2,10), 3, vec3f(0.18f)));
    objects.push_back(new Sphere(vec3f(5,2,5), 3, vec3f(0.18f)));
    
    mat44f distLightMat;
    distLightMat[2][0] = 3;
    distLightMat[2][1] = 5;
    distLightMat[2][2] = 4;
    lights.push_back(new DistantLight(distLightMat, vec3f(1.0f, 1.0f, 1.0f), 0.0f));
    
    distLightMat[3][0] = 0;
    distLightMat[3][1] = 5;
    distLightMat[3][2] = 0;
    lights.push_back(new PointLight(distLightMat, vec3f(1.0f, 0.3f, 0.3f), 3200));
    
    distLightMat[3][0] = -10;
    distLightMat[3][1] = 2;
    distLightMat[3][2] = -0.5f;
    lights.push_back(new PointLight(distLightMat, vec3f(0.3f, 0.3f, 1.0f), 5000));
    
    std::cout << "num objects: " << objects.size() << std::endl;
    
    ImageOptions options;
    options.width = 1280;
    options.height = 800;
    options.fov = 70 * DEG_TO_RAD;
    
    render(options, objects, lights);
}
