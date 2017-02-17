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

#include <math.h>

#define RAD_TO_DEG 180/M_PI
#define DEG_TO_RAD M_PI/180

template<typename T>
inline void clamp(T& val, const T& lo, const T& hi)
{
    if(val < lo)
        val = lo;
    else if(val > hi)
        val = hi;
}

#endif /* math_macros_h */
