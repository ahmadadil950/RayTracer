/* The vec3 class is used for colors, locations, directions, offsets
    Two alias are declared: point3 and color
    The utility functions are used for operations such as 
    output <<
    subtraction - 
    multiplaction * 
    scaling * 
    division /
    Dot Product (dot)
    Cross Product (cross)*/




#ifndef vec3_H
#define vec3_H

#include<cmath>
#include "rt.h"
#include<iostream>

using std::sqrt;

class vec3
{
private:
    /* data */
public:
    vec3() : e{0,0,0} {}
    vec3(float e0, float e1, float e2) : e{e0,e1,e2} {}

    float x() const 
    {
        return e[0];
    }

    float y() const
    {
        return e[1];
    }

    float z() const
    {
        return e[2];
    }

    vec3 operator-() const
    {
        return vec3(-e[0],-e[1],-e[2]);
    }

    float operator[](int i) const
    {
        return e[i];
    }

    float& operator[](int i)
    {
        return e[i];
    }

    vec3& operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*= (const float t)
    {
        /*e[0] = e[0] * t*/
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const float t)
    {
        return *this *= 1/t;
    }

    float length() const
    {
        return sqrt(length_squared());
    }

    float length_squared() const
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }


/* -------------------------------------------------------------------------------------------
First, pick a random point in the unit cube where x, y, and z all range from −1 to +1. 
Reject this point and try again if the point is outside the sphere.*/
inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

inline static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
// ---------------------------------------------------------------------------------------------

/*we'll create a new vector method — vec3::near_zero() — that returns true if the vector is very close to zero in all dimensions*/

bool near_zero() const
{
    const auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
}
    public:
        float e[3];
};

// Utility functions ---------------------------------------------------------------------------------
// u - horizontal
// v - vertical

inline std::ostream& operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' <<v.e[1]<< ' '<<v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3 &v)
{
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator/(vec3 v, double t)
{
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline float Qrt(float v)
{
    long i;
    float x2;
    float y;
    const float threehalfs = 1.5F;

    x2 = v * 0.5F;
    y = v;
    i = *(long *) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  *= * ( float * ) &i;
    y = y *(threehalfs - (x2 * y * y));
    y = y *(threehalfs - (x2 * y * y));
    return y;

}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}


/*First, pick a random point in the unit cube where x, y, and z all range from −1 to +1. 
Reject this point and try again if the point is outside the sphere.*/
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/*Lambertian distribution, which has a distribution of cos(ϕ)
True Lambertian has the probability higher for ray scattering close to the normal, but the distribution is more uniform. 
This is achieved by picking random points on the surface of the unit sphere, offset along the surface normal. 
Picking random points on the unit sphere can be achieved by picking random points in the unit sphere, and then normalizing those. */
inline vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}


/*A more intuitive approach is to have a uniform scatter direction for all angles away from the hit point, 
with no dependence on the angle from the normal. 
Many of the first raytracing papers used this diffuse method (before adopting Lambertian diffuse). */
inline vec3 random_in_hemisphere(const vec3& normal)
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    if(dot(in_unit_sphere,normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }

}


inline vec3 random_in_unit_disk()
{
    while(true)
    {
        auto p = vec3(random_double(-1,1),random_double(-1,1),0);
        if(p.length_squared() >= 1) continue;
        return p;
    }
}


/*The reflected ray direction in red is just v+2b. 
In our design, n is a unit vector, but v may not be. The length of b should be v⋅n. Because v points in, 
we will need a minus sign, yielding: */
inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2*dot(v,n)*n;
}



inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv,n),1.0);
    vec3 r_out_perpen = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -Qrt(fabs(1.0 - r_out_perpen.length_squared())) * n;
    return r_out_perpen + r_out_parallel;
}


//----------------------------------------------------------------------------------------------------
// 3D point vector
using point3 = vec3;
// RGB color vector
using color = vec3;

#endif



