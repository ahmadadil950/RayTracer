/*For our program the material needs to do two things:

    Produce a scattered ray (or say it absorbed the incident ray).
    If scattered, say how much the ray should be attenuated.
    

The hit_record is to avoid a bunch of arguments so we can stuff whatever info we want in there. 
You can use arguments instead; it’s a matter of taste. Hittables and materials need to 
know each other so there is some circularity of 
the references. 
In C++ you just need to alert the compiler that the pointer is to a class, 
which the “class material” in the hittable class below does:*/



#ifndef material_h
#define material_h

#include "rt.h"
#include "sphere.h"


struct hit_record;

class material
{
    public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};




/* it can either scatter always and attenuate by its reflectance R, or it can scatter with no attenuation 
but absorb the fraction 1−R of the rays, or it could be a mixture of those strategies.

Note we could just as well only scatter with some probability p and have attenuation be albedo/p. Your choice.


The bigger the sphere, the fuzzier the reflections will be. This suggests adding a fuzziness parameter that is just the 
radius of the sphere (so zero is no perturbation). The catch is that for big spheres or grazing rays, 
we may scatter below the surface. We can just have the surface absorb those. */



class lambertian : public material
{
    public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();
        
        if(scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p,scatter_direction);
        attenuation = albedo;
        return true;
    }

    public:
    color albedo;
};


class metal : public material {
    public:
        metal(const color& a,double f) : albedo(a), fuzz(f<1 ? f:1) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
        {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        double fuzz;
};


class diaelectric : public material {
    public:
    double ir;
    private:
    static double reflectance(double cosine,double ref_idx)
    {
        //using schlick's approx for reflectance
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1-cosine),5);
    }

    public:
    diaelectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0,1.0,1.0);
        double refraction_ratio = rec.front_Face ? (1.0/ir) : ir;
        vec3 unit_direction = unit_vector(r_in.direction());
        vec3 direction;
        double cos_theta = fmin(dot(-unit_direction,rec.normal),1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        bool cant_refract = refraction_ratio * sin_theta > 1.0;
        if(cant_refract || reflectance(cos_theta,refraction_ratio) > random_double())
        {
            direction = reflect(unit_direction,rec.normal);
        }
        else
        {
            direction = refract(unit_direction,rec.normal,refraction_ratio);
        }

        vec3 refracted = refract(unit_direction,rec.normal,refraction_ratio);

        scattered = ray(rec.p,direction);
        return true;
    }
};


#endif