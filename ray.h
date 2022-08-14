/*Ray functions: P(t) = A + tb
P(t) = 3D position on a 3D line
A = ray origin
b = ray direction
Negative values of t - go anywhere on the 3D line
Positive values of t - only get parts in front of A*/





#ifndef ray_h
#define ray_h

#include "vec3.h"

class ray
{
public:
    point3 orig;
    vec3 dir;
    ray() {}

        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}
    
    point3 origin() const 
    {
        return orig;
    }

    vec3 direction() const
    {
        return dir;
    }
    // Ray formula P(t) = A + tb
    point3 at(double t) const
    {
        return orig + t*dir;
    }

};

#endif
