/*We'll throw common useful constants and future utility functions in rtweekend.h, our general main header file


Antialising: When a real camera takes a picture, there are usually no jaggies along edges 
because the edge pixels are a blend of some foreground and some background.
We can get the same effect by averaging a bunch of samples inside each pixel.

*/





#ifndef rt_h
#define rt_h

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#include "ray.h"
#include "vec3.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

/*Antialising functions*/
#pragma acc routine seq
inline double random_double()
{
    // Return real in [0,1)
    return rand() / (RAND_MAX + 1.0);
}
#pragma acc routine seq
inline double random_double(double min, double max)
{
    //return random real in [min,max)
    return min + (max-min)*random_double();
}

// clamp(x,min,max), which clamps the value x to the range [min,max]
inline double clamp(double x, double min, double max)
{
    if(x<min)
    {
        return min;
    }

    if(x>max)
    {
        return max;
    }
    return x;
}

#endif



