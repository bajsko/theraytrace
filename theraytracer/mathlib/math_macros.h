//
//  math_macros.h
//  theraytracer
//
//  Contains some useful macros regarding math
//  e.g unit conversions
//
//  Created by Klas Henriksson on 2017-02-14.
//  Copyright © 2017 bajsko. All rights reserved.
//

#ifndef math_macros_h
#define math_macros_h

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

//Multiply an angle (measured in radians) with this number to convert it to degrees
#define RAD_TO_DEG 180/M_PI
//Multiply an angle (measured in degrees) with this number to convert it to radians
#define DEG_TO_RAD M_PI/180

//Clamp the value [val] between [lo] and [hi] (including both endpoints)
template<typename T>
inline void clamp(T& val, const T& lo, const T& hi)
{
    if(val < lo)
        val = lo;
    else if(val > hi)
        val = hi;
}

//Clamp the value [val] between [lo] and [hi] (including both endpoints) and return the clamped value without changing [val]
template<typename T>
inline T clamp_nv(const T& val, const T& lo, const T& hi)
{
	T ret = val;
	clamp(ret, lo, hi);
	return ret;
}

#endif /* math_macros_h */
