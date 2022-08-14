/*writes a single pixel's color out to the standard output stream. 


To handle the multi-sampled color computation, we'll update the write_color() function. 
Rather than adding in a fractional contribution each time we accumulate more light to the color, 
just add the full color each iteration, 
and then perform a single divide at the end (by the number of samples) when writing out the color.*/




#ifndef color_H
#define color_H

#include "vec3.h"
#include "rt.h"

#include<iostream>
#pragma acc routine seq
void write_Color(std::ostream &out, color pixel_Color, int samples_per_Pixel)
{
    auto r = pixel_Color.x();
    auto g = pixel_Color.y();
    auto b = pixel_Color.z();

    /*Dividing the color by the number of samples and gamma-correct for gamma = 2.0
    To a first approximation, we can use “gamma 2” 
    which means raising the color to the power 1/gamma, or in our simple case ½, which is just square-root: */
    auto scale = 1.0 / samples_per_Pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);



    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif