//
//  material.hpp
//  Raytracer
//
//  Created by Joe Fazzino on 19/07/2021.
//

#ifndef material_hpp
#define material_hpp

#include "rtweekend.hpp"

struct hit_record;

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

#endif /* material_hpp */
