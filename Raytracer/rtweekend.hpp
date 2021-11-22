//
//  rtweekend.hpp
//  Raytracer
//
//  Created by Joe Fazzino on 18/07/2021.
//

#ifndef rtweekend_hpp
#define rtweekend_hpp

#include <cmath>
#include <memory>
#include <limits>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max-min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


#include "ray.hpp"
#include "vec3.hpp"

#endif /* rtweekend_hpp */
