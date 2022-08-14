/*The following class implements a simple camera using the axis-aligned camera from before: */




#ifndef camera_h
#define camera_h

#include "rt.h"

class camera
{
    private:
    point3 origin;
    point3 lowerLeft_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;

    public:
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3 vup,
        double vfov,
        double aspect_ratio,
        double aperture,
        double focus_dist
        )
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup,w));
        v = cross(w,u);

        origin = lookfrom;
        horizontal = focus_dist* viewport_width * u;
        vertical = focus_dist* viewport_height * v;
        lowerLeft_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;
        lens_radius = aperture/2;
    }
    
    ray get_ray(double s, double t) const
    {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = rd.x() * u + rd.y() * v;
        return ray(origin + offset, lowerLeft_corner + s*horizontal + t*vertical - origin - offset);
    }
};

#endif