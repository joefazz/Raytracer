//
//  hittable.hpp
//  Raytracer
//
//  Created by Joe Fazzino on 17/07/2021.
//

#ifndef hittable_hpp
#define hittable_hpp

#include "ray.hpp"

using std::shared_ptr;

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;
    
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif /* hittable_hpp */
