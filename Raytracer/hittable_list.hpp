//
//  hittable_list.hpp
//  Raytracer
//
//  Created by Joe Fazzino on 18/07/2021.
//

#ifndef hittable_list_hpp
#define hittable_list_hpp

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }
    
    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    
public:
    std::vector<shared_ptr<hittable>> objects;
};

#endif /* hittable_list_hpp */
