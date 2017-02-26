//
//  main.cpp
//  theraytracer
//
//  Created by Klas Henriksson on 2017-02-13.
//  Copyright © 2017 bajsko. All rights reserved.
//

#include <iostream>
#include "vec3.h"
#include "matrix4x4.h"
#include "math_macros.h"

int main(int argc, const char * argv[]) {
    
    vec3f v1(1.0f, 0.0f, 0.0f);
    vec3f v2(5,5,5);
    mat44f m1 = Mat44Util::rotate_z<float>(90 * DEG_TO_RAD);
    m1.multVec(v1, v2);
    
    mat44f m2 = m1.inverse();

	v2 = vec3f(120, 23, 10);
	v2.normalize();
    
    Mat44Util::debug_print(m1);
    Mat44Util::debug_print(m2);
    Mat44Util::debug_print(m1 * m2);
    Vec3Util::debug_print(v2);

#ifdef _WIN32
	int i = 0;
	std::cin >> i;
#endif
}
