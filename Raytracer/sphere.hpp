//
//  sphere.hpp
//  Raytracer
//
//  Created by Joe Fazzino on 17/07/2021.
//

#ifndef sphere_hpp
#define sphere_hpp

#include "hittable.hpp"
#include "vec3.hpp"

using std::shared_ptr;

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> mat) : center(cen), radius(r), mat_ptr(mat) {};
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    
public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};

#endif /* sphere_hpp */
