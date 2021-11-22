//
//  color.hpp
//  Raytracer
//
//  Created by Joe Fazzino on 16/07/2021.
//

#ifndef color_hpp
#define color_hpp

#include "vec3.hpp"

#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel);

#endif /* color_hpp */
