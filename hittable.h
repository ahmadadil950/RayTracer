/*Abstract class for anything a ray will hit
This hittable abstract class will have a hit function that takes in a ray. Most ray tracers have found it 
convenient to add a valid interval 
for hits tmin to tmax, so the hit only “counts” if tmin<t<tmax. For the initial rays this is positive t*/

#ifndef hittable_h 
#define hittable_h

#include "ray.h"
#include "rt.h"


class material;


struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    bool front_Face;
    shared_ptr<material> mat_ptr;

    inline void set_face_Normal(const ray& r, const vec3& outward_Normal)
    {
        front_Face = dot(r.direction(),outward_Normal) < 0;
        normal = front_Face ? outward_Normal :-outward_Normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif