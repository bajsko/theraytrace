//
//  vec3.h
//  theraytracer
//
//  Represents a linear-algebraic compatible vector
//  with 3 components
//
//  Created by Klas Henriksson on 2017-02-13.
//  Copyright © 2017 bajsko. All rights reserved.
//

#ifndef vec3_h
#define vec3_h

#include <math.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "math_macros.h"

template<typename T>
class Vec3 {
public:
    
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    Vec3(const Vec3<T>& r) : x(r.x), y(r.y), z(r.z) {}
    
	Vec3<T>& operator = (const Vec3<T>& rhs)
	{
		if (this == &rhs)
			return *this;

		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;

		return *this;
	}

	Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
    Vec3<T> operator * (const T &r) const { return Vec3<T>(x * r, y * r, z * r); }
    Vec3<T>& operator *= (const T &r) { x *= r, y *= r, z *= r; return *this; }
    
    void operator += (const Vec3<T> &v) { x += v.x; y += v.y; z += v.z; }
    
	T length() const { return (x * x + y * y + z * z); }
	T lengthSquared() const { return sqrt((x * x + y * y + z * z)); }
    
	Vec3<T>& normalize()
    {
        float len = lengthSquared();
        float invLen = 1 / len;
        
        x *= invLen;
        y *= invLen;
        z *= invLen;
        
        return *this;
    }
    
	T dot(const Vec3<T>& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    
	Vec3<T> cross(const Vec3<T>& v) const
    {
        return Vec3<T>(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }
    
	T x, y, z;
};

typedef Vec3<float> vec3f;
typedef Vec3<double> vec3d;

//Small utility class to help dealing with vectors
class Vec3Util
{
public:

	//returns the angle between the two vectors a and b
	//in radians
	template<typename T>
	static T angle_between(const Vec3<T>& a, const Vec3<T>& b)
	{
		T dotVal = a.dot(b);
		T aLen = a.lengthSquared();
		T bLen = b.lengthSquared();

		T cosine = dotVal / (aLen*bLen);

		return (T)acos(cosine);
	}

	template<typename T>
	static Vec3<T> cart2spher(const Vec3<T>& v)
	{
		return Vec3<T>(acos(v.z), atan2(v.y, v.x), v.lengthSquared());
	}

	template<typename T>
	static Vec3<T> spher2cart(const T& theta, const T& phi)
	{
		return Vec3<T>(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
	}

	template<typename T>
	static inline T spher_theta(const Vec3<T>& v)
	{
		return acos(clamp<T>(v, -1, 1));
	}

	template<typename T>
	static inline T spher_phi(const Vec3<T>& v)
	{
		T p = atan2(v.y, v.x);
		return (p < 0) ? p + 2 * M_PI : p;
	}


	//Returns a normalized vector of vector v (does not normalize vector v)
	template<typename T>
	static Vec3<T> normalize(const Vec3<T>& v) {
		Vec3<T> vnorm = v;
		return vnorm.normalize();
	}

	//Performs the cross product of A and B (A x B order)
	template<typename T>
	static Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.cross(b);
	}
    
    template<typename T>
	static void debug_print(const Vec3<T>& v)
    {
        std::stringstream s;
        s << "{" << v.x << ":" << v.y << ":" << v.z << "}";
        std::cout << s.str() << std::endl;
    }
};

#endif /* vec3_h */
